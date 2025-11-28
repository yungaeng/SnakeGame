#pragma once
#define NOMINMAX

#include <algorithm>
#include <mutex>
#include <cmath>
#include "Object.h"
#include "Snake.h"

class ObjManager
{
public:
	static std::unordered_map<unsigned long long, Object> m_foods;
	static std::unordered_map<unsigned long long, Snake> m_snakes;

	void AddFood(unsigned long long id, int x, int y, COLORREF color);
	void AddSnake(unsigned long long id, wchar_t* name, int x, int y, COLORREF color);

	void DeleteFood(unsigned long long id);
	void DeleteSnake(unsigned long long id);

	// void MoveSnake(unsigned long long id, double deltaTime);
private:
	// void FoodCollisions();
	// int SnakeCollisions();
	// void GarbageCollector();
};

