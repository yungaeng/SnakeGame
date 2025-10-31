#include "pch.h"
#include "ServerManager.h"
#include "GameMap.h"

// ���ξ�����
// - AcceptThread: accept�� ����ϴ� ������
//	 - Client Thread 1
//	 - Client Thread 2
//   - Client Thread 3
//	 - ....
//	 - Client Thread N
// - gameThread: �浹����, ���� �����Ѵ�.
// - broadcastThread: 

int main()
{
	try {
		if(false == MANAGER(ServerManager)->Init())
			throw std::runtime_error("Failed IOManager Init!");

		std::jthread acceptThread{ [](const std::stop_token& st) { MANAGER(ServerManager)->DoAccept(st); } };
		std::jthread gameThread([](const std::stop_token& st) { MANAGER(GameMap)->Update(st); });
		std::jthread broadcastThread([](const std::stop_token& st) { MANAGER(ServerManager)->FlushSendBufferQueue(st); });
		
		// ���ξ�����
		std::string word;
		while(true) {
			std::cin >> word;
			if("EXIT" == word) {
				MANAGER(ServerManager)->Finish();
				break;
			}
		}

	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}