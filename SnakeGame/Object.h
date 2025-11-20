#pragma once
#include <wtypes.h>

class Object
{
public:
	int m_x;
	int m_y;
	unsigned long long m_id;
	COLORREF m_color;
	bool isalive = true;

	int m_size = 10;
	double m_speed = 20.0;
public:
	Object() { isalive = true; };
	Object(unsigned long long m_id, int x, int y, COLORREF c);
	bool CheckCollision(const Object& other) const;
};

