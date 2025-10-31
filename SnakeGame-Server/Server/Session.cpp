#include "pch.h"
#include "Session.h"

Session::Session(const uint64 id, const SOCKET socket)
	: m_id{ id }, m_socket{ socket }
{
	std::cout << std::format("Session! ID={}", m_id) << std::endl;
}

Session::~Session()
{
	shutdown(m_socket, SD_BOTH);
	std::cout << std::format("~Session! ID={}", m_id) << std::endl;
}

void Session::DoIO(const std::stop_token& st)
{
	while(false == st.stop_requested()) {
		DoRecv();	// Recv
		DoSend();	// Send
	}

	std::cout << "Finish DoIO!" << std::endl;
}

void Session::DoRecv()
{
	const int recvLen = ::recv(m_socket, m_recvBuffer.GetWritePos(), m_recvBuffer.GetFreeSize(), 0);
	
	if(recvLen == SOCKET_ERROR) {
		int32 errCode = ::WSAGetLastError();
		// 지금 recv할 게 없지만, 논블로킹 소켓으로 만들었기 떄문에 이 오류 뱉어낸다.
		if(WSAEWOULDBLOCK == errCode)
			return;
		else {		
			return;
		}
	}
	
	if(false == m_recvBuffer.OnWrite(recvLen)) {
		std::cout << "OnWrite Error" << std::endl;
		return;
	}

	const unsigned int dataSize = m_recvBuffer.GetDataSize();
	const uint32 processedLen = ProcessRecv(m_recvBuffer.GetReadPos(), dataSize);
	if(processedLen < 0 || dataSize < processedLen || m_recvBuffer.OnRead(processedLen) == false) {
		std::cout << "OnRead Error" << std::endl;
		return;
	}

	m_recvBuffer.Clean();
}

void Session::DoSend()
{
	std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
	::send(m_socket, m_sendBuffer.GetBuffer(), m_sendBuffer.GetDataSize(), 0);
	m_sendBuffer.Clear();
}

void Session::AppendToSendBuffer(std::shared_ptr<SendBuffer> sendBuffer)
{
	std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
	m_sendBuffer.Append(sendBuffer->GetBuffer(), sendBuffer->GetDataSize());
}

uint32 Session::ProcessRecv(const char* const readPos, const uint32 dataSize)
{
	uint32 processedDataLen{};

	while(true) {
		const uint32 dataLen = dataSize - processedDataLen;

		if(dataLen <= sizeof(PacketHeader)) break;

		const PacketHeader* const header = reinterpret_cast<const PacketHeader*>(&readPos[processedDataLen]);
		
		// if(dataLen < header->size) break;

		// OnRecvPacket(&buffer[processLen], header.size);

		processedDataLen += dataLen;
	}

	return processedDataLen;
}
