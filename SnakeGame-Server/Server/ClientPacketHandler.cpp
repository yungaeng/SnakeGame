#include "pch.h"
#include "ClientPacketHandler.h"

#include "Session.h"
#include "GameMap.h"
#include "Player.h"

bool Process_HANDLE_INVALID_PACKET(const std::shared_ptr<Session>& session, const char* const buffer)
{
	std::cout << "Handle_INVALID_PACKET" << std::endl;
	return false;
}

bool Process_C2S_LOGIN_PACKET(const std::shared_ptr<Session>& session, const C2S_LOGIN_PACKET& recvPkt)
{
	const bool hasName = MANAGER(GameMap)->FindName(recvPkt.name);

	// 만약, 로그인 이름이 이미 게임에 있으면 SC_LOGIN 발송
	if(hasName) {
		S2C_LOGIN_FAIL_PACKET sendPkt;
		session->AppendPkt(sendPkt);
	}
	// 로그인이 정상적으로 되었다면 게임맵에 플레이어 추가
	else {

		static std::uniform_int_distribution randomPos{ 0, 700 };
		Pos pos{ randomPos(dre), randomPos(dre) };

		S2C_LOGIN_OK_PACKET sendPkt;
		sendPkt.x = pos.x;
		sendPkt.y = pos.y;
		session->AppendPkt(sendPkt);

		auto player = std::make_shared<Player>();
		player->SetColor(recvPkt.color);
		player->SetName(recvPkt.name);
		player->SetPos(pos);
		player->SetID(MANAGER(GameMap)->GetGlobalID());
		player->SetSession(session);

		MANAGER(GameMap)->AddGameObject(std::move(player));
	}

	return true;
}