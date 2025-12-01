#pragma once

#include "SendBuffer.h"

class Session;
class SendBuffer;

class ServerManager {
	SINGLETON(ServerManager)

private:
	SOCKET													m_listenSocket{ INVALID_SOCKET };
	SOCKADDR_IN												m_serverAddr;
	
	std::mutex												m_sessionMutex;
	std::unordered_map<uint64, std::shared_ptr<Session>>	m_sessions;

	std::mutex												m_sessionThreadsMutex;
	std::vector<std::jthread>								m_sessionThreads;

public:
	bool Init();
	void Finish()noexcept;
	void Broadcast(SendBuffer* sendBuffer);

public:
	void DoAccept(const std::stop_token& st);
	void RemoveSesssion(const uint64 id);

};

