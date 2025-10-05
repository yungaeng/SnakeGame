#pragma once
#include <unordered_map>
#include "Object.h"

class ObjManager
{
public:
	static std::vector<Object> m_foods;
	static std::vector<Object> m_snake;

	ObjManager();
	~ObjManager();

	void AddFood(pos p, color c);
	void AddSnake();

	void MoveSnake(dir d);
	
	void SnakeEatFood();

	void UpDate(bool* gamestate);
private:
	void HandleCollisions();
	bool m_isgameover = false;
};

