#include "Game.h"

void Game::InitGame(HDC hdc)
{
	m_isgameover = false;
	p.SetHDC(hdc);

	o.AddSnake();
	o.AddSnake();

	// ���� �����
	for (int i = 0; i < 10; i++)
	{
		pos p = { rand() % 600, rand() % 600 };
		color c = { rand() % 255, rand() % 255,rand() % 255 };
		o.AddFood(p, c);
	}

	// Ÿ�̸� ����
	m_timer = std::chrono::steady_clock::now();
	m_last_food_spawn_time = std::chrono::steady_clock::now();
}

void Game::UpdateGame()
{
	m_isgameover = o.UpDate();

	auto now = std::chrono::steady_clock::now();
	const std::chrono::milliseconds SPAWN_INTERVAL(2000);

	if (now - m_last_food_spawn_time >= SPAWN_INTERVAL) {
		pos p{ rand() % 700, rand() % 700, 0, 0 };
		color c = { rand() % 255, rand() % 255, rand() % 255 };
		o.AddFood(p, c);

		m_last_food_spawn_time = now;
	}
}

double Game::GetElapsedTime() {
	auto now = std::chrono::steady_clock::now();
	auto duration = now - m_timer;
	return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
}

