#include "pch.h"
#include "Session.h"

#include "ServerManager.h"
#include "GameMap.h"
#include "Player.h"

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
		if(false == DoRecv()) {
			std::cout << "Session: " << m_id << " DoIO Finish!" << std::endl;
			break;
		}
		DoSend();
	}
}

bool Session::DoRecv()
{
	const int recvLen = ::recv(m_socket, m_recvBuffer.GetWritePos(), m_recvBuffer.GetFreeSize(), 0);
	
	if(recvLen == SOCKET_ERROR) {
		int32 errCode = ::WSAGetLastError();
		if(WSAEWOULDBLOCK == errCode)
			return true;
		else {
			if(WSAECONNRESET == errCode) {
				Disconnect("Client Disconnect");
				return false;
			}
		}
	}
	else if(recvLen == 0) {
		Disconnect("Recv Zero");
		return false;
	}
	
	if(false == m_recvBuffer.OnWrite(recvLen)) {
		std::cout << "OnWrite Error" << std::endl;
		return false;
	}

	const unsigned int dataSize = m_recvBuffer.GetDataSize();
	const uint32 processedLen = ProcessRecv(m_recvBuffer.GetReadPos(), dataSize);
	if(processedLen < 0 || dataSize < processedLen || m_recvBuffer.OnRead(processedLen) == false) {
		std::cout << "DoRecv Error" << std::endl;
		return false;
	}

	m_recvBuffer.Clean();

	return true;
}

void Session::DoSend()
{
	std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
	::send(m_socket, m_sendBuffer.GetBuffer(), m_sendBuffer.GetDataSize(), 0);
	m_sendBuffer.Clear();
}

void Session::AppendToSendBuffer(SendBuffer* sendBuffer)
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
		
		if(dataLen < header->packetSize) break;

		OnRecvPacket(&readPos[processedDataLen]);

		processedDataLen += dataLen;
	}

	return processedDataLen;
}

void Session::Disconnect(const std::string_view reason)
{
	MANAGER(ServerManager)->RemoveSesssion(m_id);
	auto player = m_player.lock();
	if(player) {
		MANAGER(GameMap)->AddEvent([player]()
			{
				player->SetSession(nullptr);
				MANAGER(GameMap)->RemoveGameObject(std::static_pointer_cast<Player>(player));
			});
	}
	std::cout << reason.data() << std::endl;
}

void Session::OnRecvPacket(const char* const buffer)
{
	ClientPacketHandler::HandlePacket(shared_from_this(), buffer);
}