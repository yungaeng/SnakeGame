#pragma once
#include <wtypes.h>

class Object
{
	float m_x;
	float m_y;
	int m_size = 10;
	int m_speed = 20;

	COLORREF m_color;
	bool m_isalive = true;
public:
	Object() {};
	Object(float x, float y, COLORREF c);

	void Draw(HDC hdc);

	float GetX() const { return m_x; };
	float GetY() const { return m_y; };
	int GetSpeed() const { return m_speed; };
	int GetSize() const { return m_size; };
	COLORREF GetColor() { return m_color; };

	void SetAlive(bool alive) { m_isalive = alive; }
	void SetPos(float x, float y) { m_x = x, m_y = y; };
	void SetColor(COLORREF color) { m_color = color; };
	void SetDie() { m_isalive = false; };

	bool CheckCollision(const Object& other) const;
};

