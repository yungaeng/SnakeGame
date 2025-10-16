#include "Game.h"

void Game::InitGame(HDC hdc)
{
	p.SetHDC(hdc);
	o.AddSnake();
	for (int i = 0; i < 10; i++)
	{
		pos p = { rand() % 600, rand() % 600 };
		color c = { rand() % 255, rand() % 255,rand() % 255 };
		o.AddFood(p, c);
	}
}

void Game::UpdateGame()
{
	o.UpDate(&m_isgameover);
}

void Game::EndGame()
{
}
