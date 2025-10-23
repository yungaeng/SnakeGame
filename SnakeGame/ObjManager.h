#pragma once
#include <vector>
#include "Object.h"
#include "gamedata.h"

class ObjManager
{
public:
	static std::vector<Object> m_foods;
	static std::vector<Snake> m_snakes;

	ObjManager();
	~ObjManager();

	void AddFood(pos p, color c);
	void AddSnake(const wchar_t* name);
	
	void MoveSnake(int id, dir d);

	void SnakeEatFood(int id);

	void DeleteSnake(int id);

	bool UpDate();
	bool gameover = false;
private:
	void HandleCollisions();
	void FoodCollisions();
	int SnakeCollisions();
};

