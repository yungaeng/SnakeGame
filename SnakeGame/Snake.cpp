#include "Snake.h"
#include <sstream>

void DrawName(HDC hdc, int x, int y, wchar_t name[MAX_NAME_SIZE])
{
	SetBkMode(hdc, OPAQUE);
	COLORREF textColorBg = RGB(255, 255, 200);
	SetBkColor(hdc, textColorBg);

	std::wstringstream ssOffset;
	ssOffset << L"(" << name << L")";
	std::wstring offsetText = ssOffset.str();
	TextOut(hdc, x - 2 * 10, y - 2 * 10, offsetText.c_str(), offsetText.length());
}

void Snake::Draw(HDC hdc)
{
    m_head.Draw(hdc);
    DrawName(hdc, m_head.GetX(), m_head.GetY(), m_name);
    for (auto& [id, o] : m_body)
        o.Draw(hdc);
}