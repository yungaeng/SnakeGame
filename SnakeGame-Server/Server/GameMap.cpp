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

		const auto key = std::static_pointer_cast<Player>(gameObject)->GetName();
		{
			std::lock_guard<std::mutex> lk{ m_setMutex };
			if(false == m_playerNames.contains(key))
				m_playerNames.insert(key);
		}

		std::lock_guard<std::mutex> lk{ m_mapMutex };
		for(auto& [id, obj]: m_gameObjects) {
			if(obj->GetType() == GAME_OBJECT_TYPE::FOOD) {
				S2C_FOOD_PACKET sendPkt;
				sendPkt.color = obj->GetColor();
				sendPkt.x = obj->GetPos().x;	
				sendPkt.y = obj->GetPos().y;
				session->AppendPkt(sendPkt);
			}
			else if(obj->GetType() == GAME_OBJECT_TYPE::PLAYER) {
				S2C_PLAYER_PACKET sendPkt{};
				const auto nameLen = obj->GetName().size();
				memcpy(sendPkt.name, obj->GetName().data(), nameLen * sizeof(wchar_t));
				sendPkt.name[nameLen] = L'\0';
				sendPkt.color = obj->GetColor();
				sendPkt.x = obj->GetPos().x;
				sendPkt.y = obj->GetPos().y;
				session->AppendPkt(sendPkt);

				// TODO: 기존에 있는 애들에게 새로운 플레이어 정보 전송(플레이어 정보)
				{
					S2C_PLAYER_PACKET sendPkt{};
					const auto nameLen = gameObject->GetName().size();
					memcpy(sendPkt.name, gameObject->GetName().data(), nameLen * sizeof(wchar_t));
					sendPkt.name[nameLen] = L'\0';
					sendPkt.color = gameObject->GetColor();
					sendPkt.x = gameObject->GetPos().x;
					sendPkt.y = gameObject->GetPos().y;

					// TODO: Object는 살아있는데 Session이 살아져서 발생하는 문제
					std::static_pointer_cast<Player>(obj)->GetSession()->AppendPkt(sendPkt);
				}
			}
		}
	}
	else {			
		S2C_FOOD_PACKET sendPkt;
		sendPkt.color = gameObject->GetColor();
		sendPkt.x = gameObject->GetPos().x;
		sendPkt.y = gameObject->GetPos().y;
		AppendPkt(sendPkt);
	}

	{
		std::lock_guard<std::mutex> lk{ m_mapMutex };
		m_gameObjects.try_emplace(id, std::move(gameObject));
	}
}

void GameMap::RemoveGameObject(const uint64 id)
{
	std::lock_guard<std::mutex> lk{ m_mapMutex };
	if(m_gameObjects.contains(id))
		m_gameObjects.erase(id);
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

bool GameMap::FindName(std::wstring_view name)
{
	std::lock_guard<std::mutex> lk{ m_setMutex };
	return  m_playerNames.contains(name.data());
}

void GameMap::ProcessEvent()
{
	std::lock_guard<std::mutex> lk{ m_eveMutex};
	while(false == m_eventFpQueue.empty()) {
		auto eve = m_eventFpQueue.front();
		m_eventFpQueue.pop();
		eve();
	}
}

void GameMap::AddEvent(std::function<void()> eve)
{
	std::lock_guard<std::mutex> lk{ m_eveMutex};
	m_eventFpQueue.push(eve);
}

void GameMap::CheckCollision()
{
	// TODO: 충돌체크

	// 플레이어 - 플레이어 충돌
	// SC_PLAYER_DIE
	
	// 플레이어 - 먹이
	// SC_EAT_FOOD
	// SC_REMOVE_FOOD
}

void GameMap::SpawnFood()
{
	std::cout << "SpawnFood!" << std::endl;
	auto food = std::make_shared<GameObject>(GAME_OBJECT_TYPE::FOOD);
	
	const uint64 id = MANAGER(GameMap)->GetGlobalID();
	food->SetName(L"food_" + id);
	food->SetID(id);
	
	Pos pos{ rand() % 600, rand() % 600 };
	food->SetPos(pos);
	
	const COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
	food->SetColor(c);

	AddGameObject(std::move(food));
}

void GameMap::FlushSendBuffer()
{
	std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
	const uint32 dataSize = m_sendBuffer.GetDataSize();
	MANAGER(ServerManager)->Broadcast(&m_sendBuffer);
	m_sendBuffer.Clear();
}
