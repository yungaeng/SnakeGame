#include "Game.h"

void Game::InitGame(HDC hdc)
{
	m_isgameover = false;

	// enter 패킷을 받으면 추가해 줌
	// o.AddSnake(m_userdata, rand() % 700, rand() % 700);

	// 먹이 만들기
	//for (int i = 0; i < 10; i++)
	//{
	//	int x = rand() % 600;
	//	int y = rand() % 600;
	//	COLORREF col = RGB(rand() % 256, rand() % 256, rand() % 256);
	//	o.AddFood(x, y, col);
	//}

	// 타이머 시작
	m_timer = std::chrono::steady_clock::now();
	m_last_food_spawn_time = std::chrono::steady_clock::now();
}

void Game::Update()
{
	double deltaTime = GetElapsedTime();
	for (int id = 0; id < o.m_snakes.size(); id++)
		o.MoveSnake(id, deltaTime);

	m_isgameover = o.UpDate();
	m_killer_id = o.DeathBy;

//	SpawnFood();
}

void Game::ReStart()
{
	o.gameover = false;
	m_isgameover = false;
	m_killer_id = -1;

	o.DeleteSnake(0);

	SendRestart();
}

void Game::StartBGM()
{
	PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}

void Game::StopBGM()
{
	PlaySound(NULL, NULL, 0);
}

bool Game::InitNetwork()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		MessageBox(NULL, L"WSADATA Init Error", L"Error", MB_ICONERROR);
		return false;
	}

	// 소켓 생성
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		MessageBox(NULL, L"Socket Error", L"Error", MB_ICONERROR);
		return false;
	}

	u_long non_blocking_mode = 1;
	if (ioctlsocket(m_socket, FIONBIO, &non_blocking_mode) == SOCKET_ERROR) {
		MessageBox(NULL, L"Non-Blocking Error!", L"Error", MB_ICONERROR);
		return false;
	}

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVER_PORT);
	int retval = connect(m_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR) {
		int errCode = WSAGetLastError();
		if (errCode != WSAEWOULDBLOCK) {
			// 연결 시도 자체가 불가능한 심각한 오류
			MessageBox(NULL, L"Conn	ect Error!", L"Error", MB_ICONERROR);
			return false;
		}
		// WSAEWOULDBLOCK: 연결 시도가 진행 중임 (정상적인 논블로킹 동작)
	}

	m_isconnect = true; // 연결 시도가 시작되었으므로 true
	std::thread([this]() { Recv(); }).detach();

	return true;
}

void Game::Recv()
{
	while (m_isconnect)
	{
		int recvLen = ::recv(m_socket, m_recv_buf + m_received_bytes,BUF_SIZE - m_received_bytes, 0);

		if (recvLen == 0)
		{
			// 연결 종료
			EndNetwork();
			break;
		}
		else if (recvLen < 0)
		{
			// 오류 처리 (넌블로킹 소켓이므로 WSAEWOULDBLOCK은 정상)
			int errCode = WSAGetLastError();
			if (errCode != WSAEWOULDBLOCK)
			{
				EndNetwork();
				break;
			}
			// WSAEWOULDBLOCK이 발생하면 잠시 쉬고 다시 시도 (메인 스레드 블로킹 방지)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue; // 재시도
		}

		// 2. 수신 성공: 누적 바이트 업데이트
		m_received_bytes += recvLen;

		// 3. 패킷 조립 및 처리 루프 (이전 논블로킹 로직 복구)
		while (m_received_bytes > 0)
		{
			// 헤더가 불완전하면 다음 recv를 기다립니다.
			if (m_received_bytes < sizeof(PacketHeader))
			{
				break;
			}

			PacketHeader* header = reinterpret_cast<PacketHeader*>(m_recv_buf);
			uint8_t totalSize = header->packetSize;

			// 완전한 패킷이 도착했는지 확인
			if (m_received_bytes < totalSize)
			{
				break;
			}

			// 4. 완전한 패킷 처리
			ProcessPacket(m_recv_buf);

			// 5. 처리된 패킷만큼 버퍼 이동 및 잔여 바이트 갱신
			m_received_bytes -= totalSize;
			if (m_received_bytes > 0)
			{
				memmove(m_recv_buf, m_recv_buf + totalSize, m_received_bytes);
			}
		}
	}

	// 루프 종료 시 네트워크 정리
	EndNetwork();
}

// 추출된 패킷을 처리하고 응답을 보내는 별도의 함수
void Game::ProcessPacket(char* data)
{
	// 1. 패킷 헤더에서 ID 추출
	PacketHeader* header = reinterpret_cast<PacketHeader*>(data);
	PACKET_ID pid = static_cast<PACKET_ID>(header->packetID);

	// 2. 패킷 ID에 따라 적절히 처리
	switch (pid)
	{
	case PACKET_ID::S2C_LOGIN_OK:
	{
		S2C_LOGIN_OK_PACKET* p = reinterpret_cast<S2C_LOGIN_OK_PACKET*>(data);
		o.AddSnake(m_userdata, p->x, p->y);
		SetLogin(true);
		break;
	}
	case PACKET_ID::S2C_LOGIN_FAIL:
	{
		break;
	}
	case PACKET_ID::S2C_PLAYER:
	{
		S2C_PLAYER_PACKET* p = reinterpret_cast<S2C_PLAYER_PACKET*>(data);
		UserData ud = {};
		memcpy(ud.name, p->name, 20);
		ud.color = p->color;
		o.AddSnake(ud, p->x, p->y);
		break;
	}
	case PACKET_ID::S2C_FOOD:
	{
		S2C_FOOD_PACKET* p = reinterpret_cast<S2C_FOOD_PACKET*>(data);
		o.AddFood(p->x, p->y, p->color);
		break;
	}
	case PACKET_ID::S2C_MOVE:
	{
		S2C_MOVE_PACKET* p = reinterpret_cast<S2C_MOVE_PACKET*>(data);
		o.m_snakes[p->id].m_target_x = p->x;
		o.m_snakes[p->id].m_target_y = p->y;
		//o.MoveSnake(p->id, p->deltaTime);
		break;
	}
	default:
	{
		// 알 수 없는 패킷을 받았으므로 연결을 끊거나 무시할 수 있습니다.
		break;
	}
	}
}

void Game::SendLogin()
{
	if (m_isconnect)
	{
		C2S_LOGIN_PACKET sendPkt = {};
		memcpy(sendPkt.name, m_userdata.name, sizeof(m_userdata.name));
		sendPkt.color = m_userdata.color;
		// memcpy(m_send_buf, &sendPkt, sizeof(sendPkt));
		send(m_socket, (char*)&sendPkt, sizeof(sendPkt), 0);
	}
}

void Game::SendMove(int x, int y)
{
	if (m_isconnect)
	{
		C2S_MOVE_PACKET sendPkt = {};
		sendPkt.x = x; sendPkt.y = y;
		memcpy(m_send_buf, &sendPkt, sizeof(sendPkt));
		send(m_socket, (char*)&sendPkt, sizeof(sendPkt), 0);
	}
}

void Game::SendRestart()
{
	if (m_isconnect)
	{
		C2S_RESTART_PACKET sendPkt = {};
		memcpy(m_send_buf, &sendPkt, sizeof(sendPkt));
		send(m_socket, (char*)&sendPkt, sizeof(sendPkt), 0);
	}
}

void Game::SendLeave()
{
	if (m_isconnect)
	{
		C2S_LEAVE_PACKET sendPkt = {};
		memcpy(m_send_buf, &sendPkt, sizeof(sendPkt));
		send(m_socket, (char*)&sendPkt, sizeof(sendPkt), 0);
	}
}

void Game::EndNetwork()
{
	m_isconnect = false;
	// 소켓 닫기
	closesocket(m_socket);
	WSACleanup();
}

double Game::GetElapsedTime() {
	auto now = std::chrono::steady_clock::now();
	auto duration = now - m_timer;
	return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

void Game::SpawnFood()
{
	auto now = std::chrono::steady_clock::now();
	const std::chrono::milliseconds SPAWN_INTERVAL(1000);

	if (now - m_last_food_spawn_time >= SPAWN_INTERVAL) {
		int x = rand() % 700;
		int y = rand() % 700;	

		COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
		o.AddFood(x, y, c);

		m_last_food_spawn_time = now;
	}
}

