#pragma once
#include <unordered_map>
#include "Object.h"
constexpr auto MAX_NAME_SIZE = 10;

class Snake {
	wchar_t m_name[MAX_NAME_SIZE] = {};
public:
	Snake() {};
	Snake(const wchar_t* name, float x, float y, COLORREF color)
	{
		Object h(x, y, color);
		wcscpy_s(m_name, MAX_NAME_SIZE, name);
		m_head = h;
	};

	void AddBody(UINT32 index) 
	{
		Object o(m_head.GetX(), m_head.GetY(), m_head.GetColor());
		m_body.try_emplace(index, o);
	};
	void SetBody(UINT32 index, float x, float y) { m_body[index].SetPos(x, y); };
	wchar_t* GetName() { return m_name; };

	void Draw(HDC hdc);

	std::unordered_map<UINT32, Object> m_body;
	Object m_head;

	// void Eat();
};

