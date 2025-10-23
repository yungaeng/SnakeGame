#pragma once
#include "Painter.h"
#include "KeyManager.h"
#include <mmsystem.h>
#include <chrono>

class Game
{
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
private:
	double GetElapsedTime();
	std::chrono::time_point<std::chrono::steady_clock> m_timer;
	std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
};

