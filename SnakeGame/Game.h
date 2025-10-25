#pragma once
//Network
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#pragma comment(lib, "ws2_32") //ws2_32.lib ��ũ

#define SERVER_IP "127.0.0.1" // ������ ip�ּ�
#define SERVER_PORT 9000 // ������ ��Ʈ��ȣ(������ ����)
#define BUF_SIZE 512 // �ۼ��� ������ ũ��

#include "Painter.h"
#include "ObjManager.h"
#include "KeyManager.h"
#include <mmsystem.h>
#include <chrono>

class Game
{
	//Network

	bool m_isconnect;
	SOCKET m_socket;
	char m_send_buf[BUF_SIZE];
	char m_recv_buf[BUF_SIZE];

	//------------------------------

	Painter p = {};
	ObjManager o = {};
	KeyManager k = {};
	UserData userdata = {};

public:
	bool m_isgameover = false;

	Game() {
		m_isgameover = false;
		p = {};
		o = {};
		k = {};
		userdata = {};
	};
	~Game() {};
	void InitGame(HDC hdc);
	void Draw(HDC hdc) { p.Draw(hdc); };
	void InputKey(WPARAM wParam) { k.Input(o, wParam); };

	bool IsGameOver() { return m_isgameover; };
	void UpdateGame();
	
	void ReStart();

	void StartBGM();
	void StopBGM();

	UserData* GetUserDataPtr() { return &userdata; };

	// Network

	bool InitNetwork();
	void Recv();
	void Send();
	void End();

	//------------------------------
private:
	double GetElapsedTime();
	std::chrono::time_point<std::chrono::steady_clock> m_timer;
	std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
};

