#include "pch.h"
#include "ClientPacketHandler.h"

#include "Session.h"
#include "GameMap.h"
#include "Player.h"
	
Pos GetRandomPos()
{
	static std::uniform_real_distribution<float> randomPosX{ 40.f, GameMap::MAP_WIDTH-40.f};
	static std::uniform_real_distribution<float> randomPosY{ 40.f, GameMap::MAP_HEIGHT-40.f };
	Pos pos{ randomPosX(dre), randomPosY(dre) };

	return pos;
}

bool Process_HANDLE_INVALID_PACKET(const std::shared_ptr<Session>& session, const char* const buffer)
{
	std::cout << "Handle_INVALID_PACKET" << std::endl;
	return false;
}

bool Process_C2S_LOGIN_PACKET(const std::shared_ptr<Session>& session, const C2S_LOGIN_PACKET& recvPkt)
{
	// Session Thread가 수행중

	const bool hasName = MANAGER(GameMap)->FindPlayerName(recvPkt.name);

	// 만약, 로그인 이름이 이미 게임에 있으면 SC_LOGIN 발송
	if(hasName) {
		S2C_LOGIN_FAIL_PACKET sendPkt;
		session->AppendPkt(sendPkt);
	}
	// 로그인이 정상적으로 되었다면 게임맵에 플레이어 추가
	else {

		session->SetSessionInfo(SessionInfo{ recvPkt.name, recvPkt.color });
		auto player = std::make_shared<Player>();

		const Pos pos{ GetRandomPos() };
		S2C_LOGIN_OK_PACKET sendPkt;
		sendPkt.id = player->GetID();
		sendPkt.x = pos.x;
		sendPkt.y = pos.y;
		session->AppendPkt(sendPkt);

		player->SetName(recvPkt.name);
		player->SetColor(recvPkt.color);
		player->SetPos(pos);
		player->SetSession(session);
		session->SetPlayer(player);

		MANAGER(GameMap)->AddEvent([p = std::move(player)]()
			{
				MANAGER(GameMap)->AddGameObject(std::move(p));
			});
	}

	return true;
}

bool Process_C2S_RESTART_PACKET(const std::shared_ptr<Session>& session, const C2S_RESTART_PACKET& recvPkt)
{
	const Pos pos{ GetRandomPos() };
	auto player = std::make_shared<Player>();
	S2C_LOGIN_OK_PACKET sendPkt;
	sendPkt.id = player->GetID();
	sendPkt.x = pos.x;
	sendPkt.y = pos.y;
	session->AppendPkt(sendPkt);

	player->SetName(session->GetSessionInfo().name);
	player->SetColor(session->GetSessionInfo().color);
	player->SetPos(pos);
	player->SetSession(session);
	session->SetPlayer(player);

	MANAGER(GameMap)->AddEvent([p = std::move(player)]()
		{
			MANAGER(GameMap)->AddGameObject(std::move(p));
		});
	return true;
}

bool Process_C2S_MOVE_PACKET(const std::shared_ptr<Session>& session, const C2S_MOVE_PACKET& recvPkt)
{
	auto player = session->GetPlayer();

	if(player) {
		if(player->IsAlive() == false) return false;

		const Pos pos{ recvPkt.x, recvPkt.y };
		player->SetPos(pos);

		S2C_MOVE_PACKET sendPkt;
		sendPkt.id = player->GetID();
		sendPkt.x = pos.x;
		sendPkt.y = pos.y;
		MANAGER(GameMap)->AppendPkt(sendPkt);
		return true;
	}
	else return false;

	return true;
		
}
