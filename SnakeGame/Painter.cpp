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
    for (Object obj : ObjManager::objs) {
        RECT ObjRect = { obj.m_x - obj.m_size, obj.m_y - obj.m_size, obj.m_x + obj.m_size, obj.m_y + obj.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(obj.m_r, obj.m_g, obj.m_b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }
}
