#pragma once

#define NOMINMAX

//Network
#include <winsock2.h>			
#include <ws2tcpip.h>			
#pragma comment(lib, "ws2_32")
#include "..\SnakeGame-Server\Server\pch.h"
#include <cstdlib> // wcstombs

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define BUF_SIZE 512

#include "ObjManager.h"

#include <mmsystem.h>
#include <chrono>

struct userdata {
	wchar_t name[MAX_NAME_SIZE];
	COLORREF color;
	unsigned long long id;
};

class Game {
	bool m_isconnect = false;
	bool m_islogin = false;

	SOCKET m_socket = {};
	char m_send_buf[BUF_SIZE];
	char m_recv_buf[BUF_SIZE];
	int m_received_bytes = 0;

	ObjManager o = {};
	userdata m_userdata = {};
	std::mutex game_lock;
public:
	Game()
	{
		m_isconnect = true;
		m_islogin = false;
		m_socket = INVALID_SOCKET;
		m_send_buf[0] = '\0';
		m_recv_buf[0] = '\0';
		m_userdata = {};
	};
	~Game() {};
	void Init(HDC hdc);
	void Draw(HDC hdc);
	void Input(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
		case 'Q': PostQuitMessage(0);
		}

			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			// o.m_snakes[0].m_target_x = x;
			// o.m_snakes[0].m_target_y = y;
			SendMove(x, y);
	};

	double m_time;
	void Update();
	void ReStart();

	void StartBGM();
	void StopBGM();

	void SetUserdata(userdata ud) { m_userdata = ud; };

	userdata* GetUserdata() { return &m_userdata; };	
	wchar_t* GetNameById(int id) { return o.m_snakes[id].GetName(); };
	int GetScoreById(int id) { return (int)o.m_snakes[id].m_body.size() * 10; };
	

	//  Network --------------------------------------------------------------------------

	bool InitNetwork();
	void Recv();
	void ProcessPacket(char* data);

	void SendLogin();
	void SendMove(int x, int y);
	void SendRestart();
	void SendLeave();
	
	void EndNetwork();

	bool GetConnect() { return m_isconnect; };
	bool GetLogin() { return m_islogin; };
private:
	void SetLogin(bool st) { m_islogin = st; };
	void DrawBackGround(HDC hdc);
	double GetElapsedTime();

	// void SpawnFood();
	//std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
	std::chrono::time_point<std::chrono::steady_clock> m_timer;	
};