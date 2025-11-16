#pragma once
#define NOMINMAX

#include <vector>
#include <cmath>
#include <algorithm>
#include "Object.h"
#include <unordered_map>

constexpr auto MAP_SIZE = 700;
constexpr auto MAX_NAME_SIZE = 10;

struct UserData {
	wchar_t name[MAX_NAME_SIZE];
	COLORREF color;
};
struct Snake {
	UserData userdata;
	std::vector<Object> body;
	int m_target_x, m_target_y;

	Snake() {};
	Snake(const wchar_t* name, std::vector<Object> b) { wcscpy_s(userdata.name, MAX_NAME_SIZE, name);
	body = b;
	m_target_x = b.front().m_x;
	m_target_y = b.front().m_y;
	};
};

class ObjManager
{
public:
	static std::unordered_map<unsigned long long, Object> m_foods;
	static std::unordered_map<unsigned long long, Snake> m_snakes;

	ObjManager() { gameover = false; DeathBy = -1; };

	void AddFood(unsigned long long id, int x, int y, COLORREF c);

	void AddSnake(unsigned long long id, UserData ud, int x, int y);
	void MoveSnake(unsigned long long id, double deltaTime);
	void SnakeEatFood(unsigned long long id);
	void DeleteSnake(unsigned long long id);

	bool UpDate();
	bool gameover = false;
	int DeathBy = -1;
private:
	void HandleCollisions();
	void FoodCollisions();
	int SnakeCollisions();
};

