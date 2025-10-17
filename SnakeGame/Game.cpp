#include "Game.h"

void Game::InitGame(HDC hdc)
{
	m_isgameover = false;
	p.SetHDC(hdc);

	o.AddSnake();
	o.AddSnake();

	// ∏‘¿Ã ∏∏µÈ±‚
	for (int i = 0; i < 10; i++)
	{
		pos p = { rand() % 600, rand() % 600 };
		color c = { rand() % 255, rand() % 255,rand() % 255 };
		o.AddFood(p, c);
	}
}

void Game::UpdateGame()
{
	m_isgameover = o.UpDate();
}

