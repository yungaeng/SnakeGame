#include "pch.h"
#include "GameMap.h"
#include "ServerManager.h"
#include "SendBuffer.h"

void GameMap::Update(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		// ��� player�� ���鼭 �浹 üũ �˻�
		// ���� �ֱ⸶�� ���� broadcast

		// 1�� �÷��̾ ���̸� �Ծ���
		
		// 2��, 3�� 

		// 1�� �÷��̾ ���̸� �Ծ��ٴ� ������ ��Ŷ���� ���� 2,3������ �������ش�.


		// 1. ��Ŷ�� �����

		//SC_TEST sendPkt;
		//sendPkt.a = 10;

		//// sendBuffer -> memcpy(���� ���� �ּ�, sendPkt, sizeof(sendPkt))

		//// TODO: 
		//auto sendBuffer = std::make_shared<SendBuffer>();
		//
		//MANAGER(ServerManager)->Broadcast(std::move(sendBuffer));

		// recvQueue�� ���� ������ ó��
		// �浹üũ
		// sendQueue�� ������ ����



	}

	std::cout << "Finish GameThread!" << std::endl;
}