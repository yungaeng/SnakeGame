#pragma once
#include <wtypes.h>

class Object
{
public:
	int m_x;
	int m_y;
	COLORREF m_color;
	bool isalive = true;

	int m_size = 10;
	int m_speed = 20;
public:
	Object(int x, int y, COLORREF c);
	bool CheckCollision(const Object& other) const;
};

