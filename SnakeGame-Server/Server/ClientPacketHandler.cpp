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
	// Session Thread가 수행중

	const bool hasName = MANAGER(GameMap)->FindPlayerName(recvPkt.name);

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
	// TODO: 재시작 패킷 브로드캐스트
	// TODO: 플레이어 alive =true 해주고, 위치 재설정
	auto player = session->GetPlayer();
	if(player) {
		static std::uniform_int_distribution randomPos{ 0, 700 };
		Pos pos{ randomPos(dre), randomPos(dre) };
	}
	return true;
}

bool Process_C2S_MOVE_PACKET(const std::shared_ptr<Session>& session, const C2S_MOVE_PACKET& recvPkt)
{
	auto player = session->GetPlayer();

	if(player) {
		const Pos pos{ recvPkt.x, recvPkt.y };
		player->SetPos(pos);
		std::cout << "C2S_MOVE_PACKET: PlayerID=" << player->GetID() << " Move to (" << pos.x << ", " << pos.y << ")\n";

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
