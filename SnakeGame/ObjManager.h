#pragma once
#include <vector>
#include "Object.h"

#define MAP_SIZE 700
#define MAX_NAME_SIZE 10
struct UserData {
	wchar_t name[MAX_NAME_SIZE];
	COLORREF color;
};
struct Snake {
	UserData userdata;
	std::vector<Object> body;
	Snake(const wchar_t* name, std::vector<Object> b) { wcscpy_s(userdata.name, MAX_NAME_SIZE, name); body = b; };
};

class ObjManager
{
public:
	static std::vector<Object> m_foods;
	static std::vector<Snake> m_snakes;

	ObjManager() { gameover = false; };

	void AddFood(pos p, COLORREF c);

	void AddSnake(UserData ud);
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

