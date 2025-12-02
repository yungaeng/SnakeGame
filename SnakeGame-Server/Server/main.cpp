#include "pch.h"
#include "ServerManager.h"
#include "GameMap.h"

int main()
{
	ClientPacketHandler::Init();

	try {
		if(false == MANAGER(ServerManager)->Init())
			throw std::runtime_error("Failed IOManager Init!");

		std::jthread acceptThread{ [](const std::stop_token& st) { MANAGER(ServerManager)->DoAccept(st); } };
		std::jthread gameThread([](const std::stop_token& st) { MANAGER(GameMap)->Update(st); });
		
		// 메인쓰레드
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