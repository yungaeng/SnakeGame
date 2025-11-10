#pragma once

#include "RecvBuffer.h"
#include "SendBuffer.h"

class Session : public std::enable_shared_from_this<Session> {
private:
	uint64			m_id;
	SOCKET			m_socket{ INVALID_SOCKET };
	
	RecvBuffer		m_recvBuffer;

	std::mutex		m_sendBufferMutex;
	SendBuffer		m_sendBuffer;

public:
	explicit Session(const uint64 id, const SOCKET socket);
	~Session();

public:
	SOCKET GetSocket() const noexcept { return m_socket; }

public:
	void DoIO(const std::stop_token& st);
	void AppendToSendBuffer(SendBuffer* sendBuffer);
	
	template<typename PacketType>
	void AppendPkt(PacketType&& pkt) 
	{
		std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
		m_sendBuffer.Append<PacketType>(std::forward<PacketType>(pkt));
	}

private:
	bool 	DoRecv();
	void	DoSend();
	uint32	ProcessRecv(const char* const readPos, const uint32 dataSize);
	void	Disconnect(const std::string_view reason);
	void	OnRecvPacket(const char* const buffer);
public:

};

