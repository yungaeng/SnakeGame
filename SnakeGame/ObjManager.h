#pragma once
#define NOMINMAX

#include <vector>
#include <cmath>
#include <algorithm>
#include "Object.h"
#include <mutex>

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
	unsigned long long m_id;
	Snake(const wchar_t* name, unsigned long long id, std::vector<Object> b) { wcscpy_s(userdata.name, MAX_NAME_SIZE, name);
	body = b;
	m_target_x = b.front().m_x;
	m_target_y = b.front().m_y;
	m_id = id;
	};
};

class ObjManager
{
public:
	static std::vector<Object> m_foods;
	static std::vector<Snake> m_snakes;
	static std::mutex obj_lock;

	ObjManager() { gameover = false; DeathBy = -1; };

	void AddFood(unsigned long long id, int x, int y, COLORREF c);

	void AddSnake(unsigned long long id, UserData ud, int x, int y);
	void MoveSnake(unsigned long long, double deltaTime);
	void SnakeEatFood(unsigned long long id);
	void DeleteSnake(unsigned long long id);
	void DeleteFood(unsigned long long id);
	bool UpDate();
	bool gameover = false;
	int DeathBy = -1;

	
private:
	void HandleCollisions();
	void FoodCollisions();
	int SnakeCollisions();
	void GarbageCollector();
};

