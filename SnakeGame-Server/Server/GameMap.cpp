#include "pch.h"
#include "GameMap.h"
#include "ServerManager.h"
#include "SendBuffer.h"

void GameMap::Update(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		// 모든 player들 돌면서 충돌 체크 검사
		// 일정 주기마다 먹이 broadcast

		// 1번 플레이어가 먹이를 먹었다
		
		// 2번, 3번 

		// 1번 플레이어가 먹이를 먹었다는 내용을 패킷으로 만들어서 2,3번에게 전달해준다.


		// 1. 패킷을 만든다

		//SC_TEST sendPkt;
		//sendPkt.a = 10;

		//// sendBuffer -> memcpy(버퍼 시작 주소, sendPkt, sizeof(sendPkt))

		//// TODO: 
		//auto sendBuffer = std::make_shared<SendBuffer>();
		//
		//MANAGER(ServerManager)->Broadcast(std::move(sendBuffer));

		// recvQueue에 쌓인 데이터 처리
		// 충돌체크
		// sendQueue에 데이터 전달



	}

	std::cout << "Finish GameThread!" << std::endl;
}