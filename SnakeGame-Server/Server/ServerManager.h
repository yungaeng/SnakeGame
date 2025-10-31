#pragma once

#include "LockQueue.h"

class Session;
class SendBuffer;

class ServerManager {
	SINGLETON(ServerManager)

private:
	SOCKET													m_listenSocket{ INVALID_SOCKET };
	SOCKADDR_IN												m_serverAddr;
	
	std::mutex												m_sessionMutex;
	std::unordered_map<uint64_t, std::shared_ptr<Session>>	m_sessions;

	std::mutex												m_sessionThreadsMutex;
	std::vector<std::jthread>								m_sessionThreads;

	LockQueue<std::shared_ptr<SendBuffer>>					m_sendBufferQueue;

public:
	bool Init();
	void Finish()noexcept;
	void FlushSendBufferQueue(const std::stop_token& st);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);

public:
	void DoAccept(const std::stop_token& st);

};

