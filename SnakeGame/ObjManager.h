#pragma once
#include <unordered_map>
#include "Object.h"

class ObjManager
{
public:
	static std::vector<Object> m_foods;
	static std::vector<std::vector<Object>> m_snakes;

	ObjManager();
	~ObjManager();

	void AddFood(pos p, color c);
	void AddSnake();

	void MoveSnake(int id, dir d);

	void SnakeEatFood(int id);

	void DeleteSnake(int id);

	bool UpDate();
private:
	void HandleCollisions();
	void FoodCollisions();
	void SnakeCollisions();
	bool gameover = false;
};

