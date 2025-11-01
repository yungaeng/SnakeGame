#include "Game.h"

void Game::InitGame(HDC hdc)
{
	m_isgameover = -1;

	o.AddSnake(m_userdata);

	// ���� �����
	for (int i = 0; i < 10; i++)
	{
		int x = rand() % 600;
		int y = rand() % 600;
		COLORREF col = RGB(rand() % 256, rand() % 256, rand() % 256);
		o.AddFood(x, y, col);
	}

	// Ÿ�̸� ����
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

	SpawnFood();
}

void Game::ReStart()
{
	o.gameover = false;
	m_isgameover = false;
	m_killer_id = -1;

	o.DeleteSnake(0);

	o.AddSnake(m_userdata);
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
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		MessageBox(NULL, L"WSADATA Init Error", L"Error", MB_ICONERROR);
		return false;
	}

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		MessageBox(NULL, L"Socket Error", L"Error", MB_ICONERROR);
		return false;
	}

	u_long non_blocking_mode = 1;
	if (ioctlsocket(sock, FIONBIO, &non_blocking_mode) == SOCKET_ERROR) {
		MessageBox(NULL, L"Non-Blocking Error!", L"Error", MB_ICONERROR);
		return false;
	}

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVER_PORT);
	int retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		MessageBox(NULL, L"Connect Error!", L"Error", MB_ICONERROR);
		return false;
	}

	m_isconnect = true;
	return true;
}

void Game::Recv()
{
	//recv(m_socket, m_recv_buf, sizeof(m_recv_buf), 0);
}

void Game::Send()
{
	CS_LOGIN login;
	wcstombs(login.name, m_userdata.name, sizeof(login.name));
	login.color = m_userdata.color;

	memcpy(m_send_buf, &login, sizeof(CS_LOGIN));

	if (m_isconnect)
		send(m_socket, m_send_buf, sizeof(m_send_buf), 0);
}

void Game::EndNetwork()
{
	// ���� �ݱ�
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

