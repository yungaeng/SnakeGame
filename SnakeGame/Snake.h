#pragma once
#include <vector>
#include "Object.h"
constexpr auto MAX_NAME_SIZE = 10;

class Snake {
public:
	wchar_t m_name[MAX_NAME_SIZE] = {};
	int m_target_x, m_target_y;
	
	Snake() {};
	Snake(const wchar_t* name, std::vector<Object> b)
	{
		wcscpy_s(m_name, MAX_NAME_SIZE, name);
		m_body = b;
		m_target_x = b.begin()->GetX();
		m_target_y = b.begin()->GetY();
	};

	void Draw(HDC hdc);
	void Eat();

	void SetTarget(int x, int y) { m_target_x = x, m_target_y = y; };
	
	int GetTargetX() { return m_target_x; };
	int GetTargetY() { return m_target_y; };

	wchar_t* GetName() { return m_name; };

	std::vector<Object> m_body;
};

