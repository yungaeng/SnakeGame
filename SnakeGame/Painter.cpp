#include "Painter.h"
#include "ObjManager.h"
#include <sstream>

void Painter::Draw(HDC hdc)
{
    m_hdc = hdc;
    DrawBackGround();
    DrawObject();
}

void Painter::DrawBackGround()
{
    RECT backgroundRect = { 0, 0, m_map_size, m_map_size };
    HBRUSH backgroundBrush = CreateSolidBrush(RGB(10, 10, 10));
    FillRect(m_hdc, &backgroundRect, backgroundBrush);
    DeleteObject(backgroundBrush);
}

void Painter::DrawObject()
{
    for (auto& obj : ObjManager::m_foods) {
        RECT ObjRect = { obj.m_pos.x - obj.m_size, obj.m_pos.y - obj.m_size, obj.m_pos.x + obj.m_size, obj.m_pos.y + obj.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(obj.m_color.r, obj.m_color.g, obj.m_color.b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }

    for (auto& s : ObjManager::m_snakes) {
        for (auto& o : s.body) {
            RECT ObjRect = { o.m_pos.x - o.m_size, o.m_pos.y - o.m_size, o.m_pos.x + o.m_size, o.m_pos.y + o.m_size };
            HBRUSH ObjBrush = CreateSolidBrush(RGB(o.m_color.r, o.m_color.g, o.m_color.b));
            FillRect(m_hdc, &ObjRect, ObjBrush);
            DeleteObject(ObjBrush);
        }
        // 뱀의 머리에 아이디 그리기 추가
        auto head = s.body.begin(); 
        DrawName(head->m_pos.x, head->m_pos.y, s.name, head->m_size);
    }
}

void Painter::DrawName(int x, int y, wchar_t name[MAX_NAME_SIZE], int size)
{
    SetBkMode(m_hdc, OPAQUE);
    COLORREF textColorBg = RGB(255, 255, 200);
    SetBkColor(m_hdc, textColorBg);

    std::wstringstream ssOffset;
    ssOffset << L"(" << name << L")";

    std::wstring offsetText = ssOffset.str();
    TextOut(m_hdc, x - 2 * size, y - 2 * size, offsetText.c_str(), offsetText.length());
}