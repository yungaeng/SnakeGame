#include "pch.h"
#include "ServerManager.h"

#include "Session.h"
#include "GameMap.h"

bool ServerManager::Init()
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("WSAStartup ����: %d\n", WSAGetLastError());
		return false;
	}

	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_listenSocket == INVALID_SOCKET) {
		printf("���� ���� ����\n");
		return false;
	}

	memset(&m_serverAddr, 0, sizeof(m_serverAddr));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serverAddr.sin_port = htons(SERVER_PORT);

	if(bind(m_listenSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
		printf("bind ����\n");
		return false;
	}

	if(listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen ����\n");
		return false;
	}

	return true;
}

void ServerManager::DoAccept(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(clientAddr);

		// ���� ���ŷ �����̹Ƿ�, Ŭ���̾�Ʈ���� connect�� ��û�Ǳ� �������� accept���� �����ִ�.
		// connect�� Ŭ�󿡼� �Ҹ���, accept�� ��ȯ�ȴ�.
		const SOCKET clientSocket = ::accept(m_listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

		if(clientSocket != INVALID_SOCKET) {
			std::cout << "Client Connected" << std::endl;

			// Ŭ���̾�Ʈ ������ ����ŷ���� �Ѵ�.
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
