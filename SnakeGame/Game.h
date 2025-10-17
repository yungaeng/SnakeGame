#pragma once
#include "Painter.h"
#include "KeyManager.h"
#include <chrono>

class Game
{
	Painter p = {};
	ObjManager o = {};
	KeyManager k = {};
	
public:
	bool m_isgameover = false;

	Game() {
		m_isgameover = false;
		p = {};
		o = {};
		k = {};
	};
	~Game() {};
	void InitGame(HDC hdc);
	void Draw(HDC hdc) { 
		p.Draw(hdc);
	};
	void InputKey(WPARAM wParam) { k.Input(o, wParam); };
	bool IsGameOver() { return m_isgameover; };
	void UpdateGame();
private:
	double GetElapsedTime();
	int GetLoser() { return o.loser_id; };
	std::chrono::time_point<std::chrono::steady_clock> m_timer;
	std::chrono::time_point<std::chrono::steady_clock> m_last_food_spawn_time;
};

