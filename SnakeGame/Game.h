#pragma once
#include "Painter.h"
#include "KeyManager.h"
class Game
{
	Painter p = {};
	ObjManager o = {};
	KeyManager k = {};
	bool m_isgameover = false;

public:
	Game() {
		p = {};
		o = {};
		k = {};
	};
	~Game() { EndGame(); };
	void SetGameOver(bool b) { m_isgameover = b; };

	void InitGame(HDC hdc);
	void Run(HDC hdc) { 
		UpdateGame();
		p.Draw(hdc);
	};
	void InputKey(WPARAM wParam) { k.Input(o, wParam); };
private:
	void UpdateGame();
	void EndGame();
};

