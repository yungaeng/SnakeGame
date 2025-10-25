#pragma once
#include <wtypes.h>

typedef enum Direction {
	UP, DOWN, LEFT, RIGHT
} dir;

typedef struct Position {
	int x, y;
	int prev_x, prev_y;
} pos;

class Object
{
public:
	pos m_pos;
	COLORREF m_color;
	dir m_dir = DOWN;

	int m_size = 10;
	int m_speed = 20;
public:
	Object(pos p, COLORREF c);
	bool CheckCollision(const Object& other) const;
};

