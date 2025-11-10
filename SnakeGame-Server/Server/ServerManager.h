#pragma once

#include "LockQueue.h"
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

	std::mutex												m_sendBufferMutex;
	SendBuffer												m_sendBuffer;

public:
	bool Init();
	void Finish()noexcept;
	// void FlushSendBufferQueue(const std::stop_token& st);
	void Broadcast(SendBuffer* sendBuffer);

public:
	template<typename PacketType>
	void AppendPkt(PacketType&& pkt) { m_sendBuffer.Append<PacketType>(std::forward<PacketType>(pkt)); }

public:
	void DoAccept(const std::stop_token& st);
	void RemoveSesssion(const uint64 id);

};

