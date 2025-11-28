#include "pch.h"
#include "GameMap.h"

#include "ServerManager.h"
#include "SendBuffer.h"
#include "GameObject.h"
#include "Player.h"
#include "Session.h"

void GameMap::AddGameObject(std::shared_ptr<GameObject> gameObject)
{
	const uint64 id = gameObject->GetID();

	if(GAME_OBJECT_TYPE::PLAYER == gameObject->GetType()) {
		auto player = std::static_pointer_cast<Player>(gameObject);
		auto session = player->GetSession();

		const Pos playerPos = player->GetPos();
		
		const Pos firstBodyPos{ playerPos.x - 20.f, playerPos.y - 20.f };
		player->AddBody(firstBodyPos);

		const auto key = player->GetName();
		{
			std::unique_lock<std::shared_mutex> lk{ m_nameSetMtx };
			if(false == m_playerNames.contains(key))
				m_playerNames.insert(key);
		}

		for(auto& [id, food] : m_foods) {
			S2C_FOOD_PACKET sendPkt;
			sendPkt.id = food->GetID();
			sendPkt.color = food->GetColor();
			sendPkt.x = food->GetPos().x;
			sendPkt.y = food->GetPos().y;
			session->AppendPkt(sendPkt);
		}

		{
			for(auto& [id, p] : m_players) {
				S2C_PLAYER_PACKET sendPkt{};
				const auto nameLen = p->GetName().size();
				memcpy(sendPkt.name, p->GetName().data(), nameLen * sizeof(wchar_t));
				sendPkt.name[nameLen] = L'\0';
				sendPkt.id = p->GetID();
				sendPkt.color = p->GetColor();
				sendPkt.x = p->GetPos().x;
				sendPkt.y = p->GetPos().y;
				session->AppendPkt(sendPkt);

				const auto& body = p->GetBody();

				for(int i = 0; i < body.size(); ++i) {
					S2C_ADD_SNAKE_BDOY_PACKET sendPkt;
					sendPkt.id = p->GetID();
					sendPkt.bodyIndex = i;
					sendPkt.x = body[i].x;
					sendPkt.y = body[i].y;
					session->AppendPkt(sendPkt);
				}

				{
					S2C_PLAYER_PACKET sendPkt{};
					const auto nameLen = gameObject->GetName().size();
					memcpy(sendPkt.name, gameObject->GetName().data(), nameLen * sizeof(wchar_t));
					sendPkt.name[nameLen] = L'\0';
					sendPkt.id = gameObject->GetID();
					sendPkt.color = gameObject->GetColor();
					sendPkt.x = gameObject->GetPos().x;
					sendPkt.y = gameObject->GetPos().y;

					(p)->GetSession()->AppendPkt(sendPkt);
				}

			}
		}
		m_players.try_emplace(id, std::move(std::static_pointer_cast<Player>(player)));
	}
	else if(GAME_OBJECT_TYPE::FOOD == gameObject->GetType()) {
		S2C_FOOD_PACKET sendPkt;
		sendPkt.id = gameObject->GetID();
		sendPkt.color = gameObject->GetColor();
		sendPkt.x = gameObject->GetPos().x;
		sendPkt.y = gameObject->GetPos().y;
		AppendPkt(sendPkt);
		m_foods.try_emplace(id, std::move(gameObject));
	}
}

void GameMap::RemoveGameObject(std::shared_ptr<GameObject> gameObject)
{
	const uint64 id = gameObject->GetID();

	if(gameObject->GetType() == GAME_OBJECT_TYPE::FOOD) {
		if(m_foods.contains(id)) {
			S2C_DEL_FOOD_PACKET sendPkt;
			sendPkt.id = id;
			AppendPkt(sendPkt);
			m_foods.erase(id);
		}
	}
	else if(gameObject->GetType() == GAME_OBJECT_TYPE::PLAYER) {
		if(m_players.contains(id)) {
			S2C_DEL_SNAKE_PACKET sendPkt;
			sendPkt.id = id;
			AppendPkt(sendPkt);
			const auto& name = gameObject->GetName();
			{
				std::unique_lock<std::shared_mutex> lk{ m_nameSetMtx };
				if(m_playerNames.contains(name)) {
					m_playerNames.erase(name);
				}
			}
			m_players.erase(id);
		}
	}
}

void GameMap::Update(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		m_timer.Update();
		const auto dt = m_timer.GetDT();
		m_accDTForUpdate += dt;
		m_accDTForFoodSpawn += dt;

		if(m_accDTForUpdate >= UPDATE_INVERVAL) {
			ProcessEvent();

			for(const auto& [id, player] : m_players)
				player->Update();

			CheckCollision();
			if(m_accDTForFoodSpawn >= FOOD_SPAWN_INTERVAL) {
				SpawnFood();
				m_accDTForFoodSpawn = 0.f;
			}

			FlushSendBuffer();
			m_accDTForUpdate = 0.f;
		}
	}

	std::cout << "Finish GameThread!" << std::endl;
}

bool GameMap::FindPlayerName(std::wstring_view name)
{
	std::shared_lock<std::shared_mutex> lk{ m_nameSetMtx };
	return  m_playerNames.contains(name.data());
}

void GameMap::ProcessEvent()
{
	std::lock_guard<std::recursive_mutex> lk{ m_eveMtx };
	while(false == m_eventFpQueue.empty()) {
		auto eve = m_eventFpQueue.front();
		m_eventFpQueue.pop();
		eve();
	}
}

void GameMap::AddEvent(std::function<void()> eve)
{
	std::lock_guard<std::recursive_mutex> lk{ m_eveMtx };
	m_eventFpQueue.push(eve);
}

void GameMap::CheckCollision()
{
	for(auto pIter1 = m_players.begin(); pIter1 != m_players.end(); ++pIter1) {
		const auto& curPlayer = pIter1->second;
		if(false == curPlayer->IsAlive()) continue;

		auto pIter2 = pIter1;
		++pIter2;

		for(; pIter2 != m_players.end(); ++pIter2) {
			const auto& other = pIter2->second;
			if(false == other->IsAlive()) continue;

			if(curPlayer->IsCollision(other->GetPos())) {
				S2C_DEL_SNAKE_PACKET sendPkt;
				sendPkt.id = curPlayer->GetID();
				AppendPkt(sendPkt);
			}

			const auto& otherBody = other->GetBody();
			for(const auto otherBodyPos : otherBody) {
				if(curPlayer->IsCollision(otherBodyPos)) {
					S2C_DEL_SNAKE_PACKET sendPkt;
					sendPkt.id = curPlayer->GetID();
					AppendPkt(sendPkt);
					break;
				}
			}
		}

		for(const auto& [foodID, food] : m_foods) {
			if(false == food->IsAlive()) continue;

			if(curPlayer->IsCollision(food->GetPos())) {
				std::cout << "Eat Food!" << std::endl;
				food->SetAlive(false);

				S2C_DEL_FOOD_PACKET sendPkt;
				sendPkt.id = foodID;
				AppendPkt(sendPkt);
				
				AddEvent([this, f = food]() { RemoveGameObject(f); });

				{
					const auto& body = curPlayer->GetBody();
					if(body.size() == 0) break;
					Pos newBodyPos{ body.back().x -20.f, body.back().y- 20.f };
					curPlayer->AddBody(newBodyPos);
				}
			}
		}
	}
}

void GameMap::SpawnFood()
{
	std::cout << "SpawnFood!" << std::endl;
	auto food = std::make_shared<GameObject>(GAME_OBJECT_TYPE::FOOD);

	const uint64 id = MANAGER(GameMap)->GetGlobalID();
	food->SetName(L"food_" + id);
	food->SetID(id);

	Pos pos{ rand() % GameMap::MAP_SIZE, rand() % GameMap::MAP_SIZE };
	food->SetPos(pos);
	static constexpr int MAX_COLOR{ 256 };

	const COLORREF c = RGB(rand() % MAX_COLOR, rand() % MAX_COLOR, rand() % MAX_COLOR);
	food->SetColor(c);

	AddEvent([this, f = std::move(food)]() { AddGameObject(std::move(f)); });
}

void GameMap::FlushSendBuffer()
{
	std::lock_guard<std::mutex> lk{ m_sendBuffMtx };
	const uint32 dataSize = m_sendBuffer.GetDataSize();
	MANAGER(ServerManager)->Broadcast(&m_sendBuffer);
	m_sendBuffer.Clear();
}
