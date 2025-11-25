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
	int x = m_body.begin()->GetX();
	int y = m_body.begin()->GetY();
	for (auto& o : m_body)
	{
		o.Draw(hdc);
	}
	DrawName(hdc, x, y, m_name);
}

void Snake::Eat()
{
    if (m_body.size() < 2) {
        m_body.emplace_back(Object(m_body.front().GetX(), m_body.front().GetY(), m_body.front().GetColor()));
        return;
    }

    auto tail = m_body[m_body.size() - 1];
    auto prev_tail = m_body[m_body.size() - 2];
    const double SEGMENT_SIZE = tail.GetSize();

    double dx = prev_tail.GetX() - tail.GetX();
    double dy = prev_tail.GetY() - tail.GetY();
    double distance = std::sqrt(dx * dx + dy * dy);

    double unit_dx = (distance != 0.0) ? dx / distance : 0.0;
    double unit_dy = (distance != 0.0) ? dy / distance : 0.0;


    double new_x = tail.GetX() - (unit_dx * SEGMENT_SIZE); // 部府->菊部府 氦磐甫 画
    double new_y = tail.GetY() - (unit_dy * SEGMENT_SIZE); // 部府->菊部府 氦磐甫 画

    m_body.emplace_back(Object(new_x, new_y, m_body.front().GetColor()));
}

