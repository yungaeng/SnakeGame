#pragma once

typedef enum Direction {
	UP, DOWN, LEFT, RIGHT, NOUN
} dir;

typedef struct Position {
	int x, y;
	int prev_x, prev_y;
} pos;

typedef struct Color {
	int r, g, b;
} color;

class Object
{
public:
	pos m_pos;
	color m_color;
	dir m_dir = NOUN;

	int m_size = 10;
	int m_speed = 20;
	int m_target = -1;
public:
	Object(pos p, color c);
	bool CheckCollision(const Object& other) const;
};

