#pragma once

class Object
{
public:
	int m_x, m_y;
	int m_r, m_g, m_b;
	int m_size;
	int m_speed = 10;
	char m_dir;
	int m_target = -1;
	int m_tail_len = 10;
public:
	Object(int x, int y, int size, int r, int g,  int b);
	bool CheckCollision(const Object& other) const;
};

