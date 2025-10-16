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
    RECT backgroundRect = { 0, 0, m_map_size, m_map_size };
    HBRUSH backgroundBrush = CreateSolidBrush(RGB(10, 10, 10));
    FillRect(m_hdc, &backgroundRect, backgroundBrush);
    DeleteObject(backgroundBrush);
}

void Painter::DrawObject()
{
    for (auto &obj : ObjManager::m_foods) {
        if (obj.m_islive) {
            RECT ObjRect = { obj.m_pos.x - obj.m_size, obj.m_pos.y - obj.m_size, obj.m_pos.x + obj.m_size, obj.m_pos.y + obj.m_size };
            HBRUSH ObjBrush = CreateSolidBrush(RGB(obj.m_color.r, obj.m_color.g, obj.m_color.b));
            FillRect(m_hdc, &ObjRect, ObjBrush);
            DeleteObject(ObjBrush);
        }
    }

    for (auto &o : ObjManager::m_snake) {
        RECT ObjRect = { o.m_pos.x - o.m_size, o.m_pos.y - o.m_size, o.m_pos.x + o.m_size, o.m_pos.y + o.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(o.m_color.r, o.m_color.g, o.m_color.b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }

    for (auto& o : ObjManager::m_other_snake) {
        RECT ObjRect = { o.m_pos.x - o.m_size, o.m_pos.y - o.m_size, o.m_pos.x + o.m_size, o.m_pos.y + o.m_size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(o.m_color.r, o.m_color.g, o.m_color.b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }
}
