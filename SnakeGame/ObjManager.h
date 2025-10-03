#pragma once
#include <unordered_map>
#include "Object.h"

class ObjManager
{
public:
	static std::unordered_map<int, Object> objs;

	ObjManager() {};
	void AddObj(int id, int x, int y, int size, int r, int g, int b);
	void AddObjRandom(int id);
	void MoveObj(int index, char dir);
	void UpdateObj();

private:
	void HandleCollisions();
};

