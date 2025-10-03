#pragma once

class Object
{
public:
	int m_x, m_y;
	int m_prev_x, m_prev_y;

	int m_id;
	int m_r, m_g, m_b;
	int m_size;
	int m_speed = 20;
	char m_dir;

	int m_target = -1;
public:
	Object() {};
	Object(int id, int x, int y, int size, int r, int g,  int b);
	bool CheckCollision(const Object& other) const;
};

