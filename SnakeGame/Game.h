#pragma once
//Network
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#pragma comment(lib, "ws2_32") //ws2_32.lib 링크

#define SERVER_IP "127.0.0.1" // 서버의 ip주소
#define SERVER_PORT 9000 // 서버의 포트번호(서버와 동일)
#define BUF_SIZE 512 // 송수신 버퍼의 크기

#include "Painter.h"
#include "ObjManager.h"
#include "UIManager.h"
#include <mmsystem.h>
#include <chrono>

class Game
{
	bool m_isconnect;
	SOCKET m_socket;
	char m_send_buf[BUF_SIZE];
	char m_recv_buf[BUF_SIZE];

	Painter p = {};
	ObjManager o = {};
	UIManager k = {};
public:
	bool m_isgameover = false;
	int m_killer_id = -1;
	UserData userdata = {};

	Game() {
		m_isgameover = false;
		m_killer_id = -1;
		p = {};
		o = {};
		k = {};
		userdata = {};
	};
	~Game() {};
	void InitGame(HDC hdc);
	void Draw(HDC hdc) { p.Draw(hdc); };
	void Input(WPARAM wParam, LPARAM lParam) { k.Input(o, wParam); k.MouseInput(o, lParam); };

	void Update();
	
	void ReStart();

	void StartBGM();
	void StopBGM();
	wchar_t* GetNameById(int id) { return o.m_snakes[id].userdata.name; };
	
	bool InitNetwork();
	void Recv();
	void Send();
	void EndNetwork();

	//------------------------------
private:
	double GetElapsedTime();
	std::chrono::time_point<std::chrono::steady_clock> m_timer;
	std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
};

