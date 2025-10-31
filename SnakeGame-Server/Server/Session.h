#pragma once

#include "RecvBuffer.h"
#include "SendBuffer.h"

class Session {
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
	void AppendToSendBuffer(std::shared_ptr<SendBuffer> sendBuffer);

private:
	void DoRecv();
	void DoSend();
	uint32 ProcessRecv(const char*  const readPos, const uint32 dataSize);

public:

};

