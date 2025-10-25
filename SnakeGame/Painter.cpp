#include "Painter.h"

void Painter::Draw(HDC hdc)
{
    DrawBackGround(hdc);
    DrawObject(hdc);
}

void Painter::DrawBackGround(HDC hdc)
{
    RECT backgroundRect = { 0, 0, m_map_size, m_map_size };
    HBRUSH backgroundBrush = CreateSolidBrush(RGB(10, 10, 10));
    FillRect(hdc, &backgroundRect, backgroundBrush);
    DeleteObject(backgroundBrush);
}

void Painter::DrawObject(HDC hdc)
{
    for (auto& obj : ObjManager::m_foods) {
        RECT ObjRect = { obj.m_pos.x - obj.m_size, obj.m_pos.y - obj.m_size, obj.m_pos.x + obj.m_size, obj.m_pos.y + obj.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(obj.m_color);
        FillRect(hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }

    for (auto& s : ObjManager::m_snakes) {
        for (auto& o : s.body) {
            RECT ObjRect = { o.m_pos.x - o.m_size, o.m_pos.y - o.m_size, o.m_pos.x + o.m_size, o.m_pos.y + o.m_size };
            HBRUSH ObjBrush = CreateSolidBrush(o.m_color);
            FillRect(hdc, &ObjRect, ObjBrush);
            DeleteObject(ObjBrush);
        }
        // 뱀의 머리에 아이디 그리기 추가
        auto head = s.body.begin(); 
        DrawName(hdc, head->m_pos.x, head->m_pos.y, s.userdata.name, head->m_size);
    }
}

void Painter::DrawName(HDC hdc, int x, int y, wchar_t name[MAX_NAME_SIZE], int size)
{
    SetBkMode(hdc, OPAQUE);
    COLORREF textColorBg = RGB(255, 255, 200);
    SetBkColor(hdc, textColorBg);

    std::wstringstream ssOffset;
    ssOffset << L"(" << name << L")";

    std::wstring offsetText = ssOffset.str();
    TextOut(hdc, x - 2 * size, y - 2 * size, offsetText.c_str(), offsetText.length());
}