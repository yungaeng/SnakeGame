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
    if (!m_body.empty()) {
        int x = m_body.begin()->GetX();
        int y = m_body.begin()->GetY();

        for(int i = 0; i < m_body.size(); ++i) {
            if(i==0) {
                m_body[i].SetPos(m_target_x, m_target_y);
            }
            m_body[i].Draw(hdc);
        }

        DrawName(hdc, x, y, m_name);
    }
}

void Snake::AddBody(int x, int y)
{
    COLORREF mycolor = m_body.begin()->GetColor();
    m_body.emplace_back(Object(x, y, mycolor));
}

// Snake::Eat() (std::vector<Object> m_body 사용 가정)

void Snake::Eat()
{
    COLORREF mycolor = m_body.begin()->GetColor();

    if (m_body.size() == 0) {
        // 비어있는 경우는 없겠지만, 안전을 위해 처리
        return;
    }

    if (m_body.size() == 1)
    {
        // 세그먼트가 1개일 경우, 머리 위치에 새 꼬리를 추가. 
        // 다음 이동 시 자동으로 분리됩니다.
        m_body.emplace_back(Object(
            m_body.begin()->GetX(),
            m_body.begin()->GetY(),
            mycolor
        ));
        return;
    }

    // 꼬리 세그먼트와 그 앞 세그먼트의 위치를 참조합니다.
    const auto& tail = m_body.back(); // 벡터의 마지막 요소
    const auto& prev_tail = m_body.at(m_body.size() - 2);
    const double SEGMENT_SIZE = tail.GetSize();

    // prev_tail에서 tail로 향하는 벡터 (direction: prev -> tail)
    double dx = tail.GetX() - prev_tail.GetX(); // 순서 변경: tail - prev_tail
    double dy = tail.GetY() - prev_tail.GetY(); // 순서 변경: tail - prev_tail
    double distance = std::sqrt(dx * dx + dy * dy);

    // 단위 벡터 계산
    double unit_dx = (distance != 0.0) ? dx / distance : 0.0;
    double unit_dy = (distance != 0.0) ? dy / distance : 0.0;

    // 새 꼬리 위치: tail에서 *prev_tail의 반대 방향*으로 한 세그먼트 크기만큼 이동
    // 즉, (tail의 위치) + (unit_vector * SEGMENT_SIZE)
    double new_x = tail.GetX() + (unit_dx * SEGMENT_SIZE);
    double new_y = tail.GetY() + (unit_dy * SEGMENT_SIZE);

    // 새 꼬리를 벡터의 맨 뒤에 추가합니다.
    m_body.emplace_back(Object(new_x, new_y, mycolor));
}