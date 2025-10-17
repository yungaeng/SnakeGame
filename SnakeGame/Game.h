#pragma once
#include "Painter.h"
#include "KeyManager.h"
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
	void SetGameOver(bool b) { m_isgameover = b; };

	void InitGame(HDC hdc);
	void Run(HDC hdc) { 
	    UpdateGame();
		p.Draw(hdc);
	};
	void InputKey(WPARAM wParam) { k.Input(o, wParam); };
	bool IsGameOver() { return m_isgameover; };
private:
	void UpdateGame();
};

