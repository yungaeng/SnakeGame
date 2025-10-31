#include "pch.h"
#include "ServerManager.h"

#include "Session.h"
#include "GameMap.h"

bool ServerManager::Init()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("WSAStartup 실패: %d\n", WSAGetLastError());
		return false;
	}

	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_listenSocket == INVALID_SOCKET) {
		printf("소켓 생성 실패\n");
		return false;
	}

	memset(&m_serverAddr, 0, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serverAddr.sin_port = htons(SERVER_PORT);

	if(bind(m_listenSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
		printf("bind 실패\n");
		return false;
	}

	if(listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen 실패\n");
		return false;
	}

	return true;
}

void ServerManager::DoAccept(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);

		// 현재 블로킹 소켓이므로, 클라이언트에서 connect가 요청되기 전까지는 accept에서 잠들고있다.
		// connect가 클라에서 불리면, accept가 반환된다.
		const SOCKET clientSocket = ::accept(m_listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

		if(clientSocket != INVALID_SOCKET) {
			std::cout << "Client Connected" << std::endl;

			// 클라이언트 소켓은 논블로킹으로 한다.
			u_long on = 1;
			if(::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
				std::cout << "ioctlsocket Failed" << std::endl;

			const uint64 id = MANAGER(GameMap)->GetGlobalID();
			auto session = std::make_shared<Session>(id, clientSocket);
			{
				std::lock_guard<std::mutex> lk{ m_sessionMutex };
				m_sessions.try_emplace(id, session);
			}
			{
				std::lock_guard<std::mutex> lk{ m_sessionThreadsMutex };
				m_sessionThreads.emplace_back([this, session](const std::stop_token& st) { session->DoIO(st); });
			}
		}
		else break;
	}

	std::cout << "Finish Accept Thread" << std::endl;
}

void ServerManager::Finish() noexcept
{
	shutdown(m_listenSocket, SD_BOTH);
	WSACleanup();
	{
		std::lock_guard<std::mutex> lk{ m_sessionThreadsMutex };
		m_sessionThreads.clear();
	}
	std::cout << "IOManager Finish" << std::endl;
}

void ServerManager::FlushSendBufferQueue(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		while(m_sendBufferQueue.Empty() != false) {
			auto sendBuffer = m_sendBufferQueue.Pop();
			{
				if(sendBuffer) {
					std::lock_guard<std::mutex> lk{ m_sessionMutex };
					for(auto& [id, session] : m_sessions) {
						session->AppendToSendBuffer(sendBuffer);
					}
				}
			}
		}
	}

	std::cout << "Finish BroadcastThread" << std::endl;
}

void ServerManager::Broadcast(std::shared_ptr<SendBuffer> sendBuffer)
{
	m_sendBufferQueue.Push(sendBuffer);
}
