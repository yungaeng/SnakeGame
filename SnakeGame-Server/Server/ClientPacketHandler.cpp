#include "pch.h"
#include "ClientPacketHandler.h"

#include "Session.h"
#include "GameMap.h"
#include "Player.h"

Pos GetRandomPos()
{
	static std::uniform_real_distribution<float> randomPosX{ 0, GameMap::MAP_WIDTH };
	static std::uniform_real_distribution<float> randomPosY{ 0, GameMap::MAP_HEIGHT };
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

		const Pos pos{ GetRandomPos() };
		S2C_LOGIN_OK_PACKET sendPkt;
		sendPkt.id = session->GetID();
		sendPkt.x = pos.x;
		sendPkt.y = pos.y;
		session->AppendPkt(sendPkt);

		auto player = std::make_shared<Player>();
		player->SetColor(recvPkt.color);
		player->SetName(recvPkt.name);
		player->SetPos(pos);
		player->SetID(session->GetID());
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
	// TODO: 재시작 구현
	auto player = session->GetPlayer();
	
	if(player->IsAlive()) return false;

	if(player) {
		const Pos pos{ GetRandomPos() };
		player->SetPos(pos);
		player->SetAlive(true);
	}
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
