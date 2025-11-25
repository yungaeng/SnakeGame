#include "Game.h"

void Game::Init(HDC hdc)
{
	// 이제 loginok 패킷을 받으면 추가해 줌 
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
	//m_last_food_spawn_time = std::chrono::steady_clock::now();
}
void Game::Draw(HDC hdc)
{
	DrawBackGround(hdc);

	o.obj_lock.lock();
	for (auto& f : o.m_foods)
		f.second.Draw(hdc);
	
	for (auto& s : o.m_snakes)
		s.second.Draw(hdc);
	o.obj_lock.unlock();
}
void Game::Update()
{
	double deltaTime = GetElapsedTime();
	o.obj_lock.lock();
	for(auto& snake : o.m_snakes) {
		o.MoveSnake(snake.first, deltaTime);
	}
	o.obj_lock.unlock();
	// 이제 게임오버 처리는 recv 에서
	// SpawnFood();
}
void Game::ReStart()
{
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
			MessageBox(NULL, L"Connect Error!", L"Error", MB_ICONERROR);
			return false;
		}
		else { 
			std::this_thread::sleep_for(2s); 
		};
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
		Object obj(p->x, p->y, m_userdata.color);
		std::vector<Object> v;
		v.emplace_back(obj);
		Snake s = { m_userdata.name, v };
		o.AddSnake(p->id, s);
		m_userdata.id = p->id;
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
		Object obj(p->x, p->y, p->color);
		std::vector<Object> v;
		v.emplace_back(obj);
		Snake s = { p->name, v };
		o.AddSnake(p->id, s);
		break;
	}
	case PACKET_ID::S2C_FOOD:
	{
		S2C_FOOD_PACKET* p = reinterpret_cast<S2C_FOOD_PACKET*>(data);
		Object f(p->x, p->y, p->color);
		o.AddFood(p->id, f);
		break;
	}
	case PACKET_ID::S2C_MOVE:
	{
		S2C_MOVE_PACKET* p = reinterpret_cast<S2C_MOVE_PACKET*>(data);
		o.obj_lock.lock();
		o.m_snakes[p->id].SetTarget(p->x, p->y);
		o.obj_lock.unlock();
		break;
	}
	case PACKET_ID::S2C_DEL_SNAKE:
	{
		S2C_DEL_SNAKE_PACKET* p = reinterpret_cast<S2C_DEL_SNAKE_PACKET*>(data);
		o.DeleteSnake(p->id);
		break;
	}
	case PACKET_ID::S2C_DEL_FOOD:
	{
		S2C_DEL_FOOD_PACKET* p = reinterpret_cast<S2C_DEL_FOOD_PACKET*>(data);
		o.DeleteFood(p->id);
		break;
	}
	case PACKET_ID::S2C_EAT_FOOD:
	{
		S2C_EAT_FOOD_PACKET* p = reinterpret_cast<S2C_EAT_FOOD_PACKET*>(data);
		o.obj_lock.lock();
		o.m_snakes[p->id].Eat();
		o.obj_lock.unlock();
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
	if (m_isconnect && m_islogin)
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

void Game::DrawBackGround(HDC hdc)
{
	RECT backgroundRect = { 0, 0, 700, 700 };
	HBRUSH backgroundBrush = CreateSolidBrush(RGB(10, 10, 10));
	FillRect(hdc, &backgroundRect, backgroundBrush);
	DeleteObject(backgroundBrush);
	
	// ID 0 뱀이 존재하는 경우에만 점수 계산
	int score = 0;
	if (o.m_snakes.count(0)) {
		score = (int)o.m_snakes[m_userdata.id].m_body.size() * 10;
	}
	
	// --- 텍스트 출력 추가 부분 ---
	char textBuffer[50];
	sprintf_s(textBuffer, sizeof(textBuffer), "FOODS : %d | SNAKES : %d | SCORE : %d",
		(int)o. m_foods.size(), (int)o.m_snakes.size(), (int)o.m_snakes[m_userdata.id].m_body.size() * 10);
	SetBkMode(hdc, OPAQUE);
	COLORREF textColorBg = RGB(255, 255, 200);
	SetBkColor(hdc, textColorBg);
	TextOutA(hdc, 10, 10, textBuffer, (int)strlen(textBuffer));
}


double Game::GetElapsedTime() {
	auto now = std::chrono::steady_clock::now();
	auto duration = now - m_timer;
	return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

//void Game::SpawnFood()
//{
//	auto now = std::chrono::steady_clock::now();
//	const std::chrono::milliseconds SPAWN_INTERVAL(1000);
//
//	/*if (now - m_last_food_spawn_time >= SPAWN_INTERVAL) {
//		int x = rand() % 700;
//		int y = rand() % 700;	
//
//		COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
//		o.AddFood(x, y, c);
//
//		m_last_food_spawn_time = now;
//	}*/
//}