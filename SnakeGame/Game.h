#pragma once
//Network
#include <winsock2.h>			// ����2 ���� ���
#include <ws2tcpip.h>			// ����2 Ȯ�� ���
#pragma comment(lib, "ws2_32")	//ws2_32.lib ��ũ
#include "..\protocol\protocol.h"
#include <cstdlib> // For wcstombs

#define SERVER_IP "192.168.64.59"	// ������ ip�ּ�
#define SERVER_PORT 9000		// ������ ��Ʈ��ȣ
#define BUF_SIZE 512			// �ۼ��� ������ ũ��

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
	UserData m_userdata = {};

	Game() {
		m_isconnect = false;
		m_socket = {};
		m_send_buf[0] = '\0';
		m_recv_buf[0] = '\0';

		m_isgameover = false;
		m_killer_id = -1;
		p = {};
		o = {};
		k = {};
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
	int GetScoreById(int id) { return (int)o.m_snakes[id].body.size() * 10; };

	bool InitNetwork();
	void Recv();
	void Send();
	void EndNetwork();

private:
	double GetElapsedTime();
	void SpawnFood();
	std::chrono::time_point<std::chrono::steady_clock> m_timer;
	std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
};

