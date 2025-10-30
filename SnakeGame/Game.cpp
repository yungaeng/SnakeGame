#include "Game.h"

void Game::InitGame(HDC hdc)
{
	m_isgameover = -1;

	o.AddSnake(userdata);

	// 먹이 만들기
	for (int i = 0; i < 10; i++)
	{
		pos p = { rand() % 600, rand() % 600 };
		COLORREF col = RGB(rand() % 256, rand() % 256, rand() % 256);
		o.AddFood(p, col);
	}

	// 타이머 시작
	m_timer = std::chrono::steady_clock::now();
	m_last_food_spawn_time = std::chrono::steady_clock::now();
}

void Game::UpdateGame()
{
	m_isgameover = o.UpDate();
	m_killer_id = o.DeathBy;

	auto now = std::chrono::steady_clock::now();
	const std::chrono::milliseconds SPAWN_INTERVAL(1000);

	if (now - m_last_food_spawn_time >= SPAWN_INTERVAL) {
		pos p{ rand() % 700, rand() % 700, 0, 0 };
		COLORREF c = RGB(rand() % 256, rand() % 256, rand() % 256);
		o.AddFood(p, c);

		m_last_food_spawn_time = now;
	}
}

void Game::ReStart()
{
	o.gameover = false;
	m_isgameover = false;
	m_killer_id = -1;

	o.DeleteSnake(0);
	o.AddSnake(userdata);
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
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) return false;

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVER_PORT);
	int retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return false;

	return true;
}

void Game::Recv()
{
	recv(m_socket, m_recv_buf, sizeof(m_recv_buf), 0);
}

void Game::Send()
{
	send(m_socket, m_recv_buf, sizeof(m_recv_buf), 0);
}

void Game::End()
{
	// 소켓 닫기
	closesocket(m_socket);
	WSACleanup();
}

double Game::GetElapsedTime() {
	auto now = std::chrono::steady_clock::now();
	auto duration = now - m_timer;
	return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

