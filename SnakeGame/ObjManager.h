#pragma once
#include <vector>
#include "Object.h"

class ObjManager
{
public:
	static std::vector<Object> objs;

	ObjManager() {};
	void AddObj(int x, int y, int size, int r, int g, int b);
	void AddObjRandom();
	void MoveObj(int index, char dir);
	void UpdateObj();

private:
	void HandleCollisions();
};

