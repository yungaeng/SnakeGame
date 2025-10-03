#include "Painter.h"
#include "ObjManager.h"

void Painter::Draw(HDC hdc)
{
    m_hdc = hdc;
    DrawBackGround();
    DrawObject();
}

void Painter::DrawBackGround()
{
    RECT backgroundRect = { 0, 0, map_size, map_size };
    HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(m_hdc, &backgroundRect, backgroundBrush);
    DeleteObject(backgroundBrush);
}


void Painter::DrawObject()
{
    for (auto obj : ObjManager::objs) {
        RECT ObjRect = { obj.second.m_x - obj.second.m_size, obj.second.m_y - obj.second.m_size, obj.second.m_x + obj.second.m_size, obj.second.m_y + obj.second.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(obj.second.m_r, obj.second.m_g, obj.second.m_b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }

    for (auto o : ObjManager::snake) {
        RECT ObjRect = { o.m_x - o.m_size, o.m_y - o.m_size, o.m_x + o.m_size, o.m_y + o.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(o.m_r, o.m_g, o.m_b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }
}
