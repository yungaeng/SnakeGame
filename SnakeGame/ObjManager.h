#pragma once
#define NOMINMAX

#include <algorithm>
#include <mutex>
#include <cmath>
#include "unordered_map"
#include "Object.h"
#include "Snake.h"

class ObjManager
{
public:
	static std::unordered_map<unsigned long long, Object> m_foods;
	static std::unordered_map<unsigned long long, Snake> m_snakes;
	std::mutex obj_lock;

	void AddFood(unsigned long long id, Object o);
	void AddSnake(unsigned long long id, Snake s);

	void MoveSnake(unsigned long long id, double deltaTime);

	void DeleteFood(unsigned long long id);
	void DeleteSnake(unsigned long long id);

	//bool UpDate();
private:
	/*void HandleCollisions();
	void FoodCollisions();
	int SnakeCollisions();
	void GarbageCollector();*/
};

