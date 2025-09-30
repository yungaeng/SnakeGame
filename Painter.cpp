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
    for (auto& obj : ObjManager::objs) {
        int x, y, size, r, g, b;
        if (obj.m_head_id == -1) {
            x = obj.m_x;
            y = obj.m_y;
        }
        else {
            x = ObjManager::objs[obj.m_head_id].m_x;
            y = ObjManager::objs[obj.m_head_id].m_y;

            switch (ObjManager::objs[0].m_dir)
            {
            case 0: y = ObjManager::objs[obj.m_head_id].m_y + ObjManager::objs[0].tail; break;
            case 1: y = ObjManager::objs[obj.m_head_id].m_y - ObjManager::objs[0].tail; break;
            case 2: x = ObjManager::objs[obj.m_head_id].m_x + ObjManager::objs[0].tail; break;
            case 3: x = ObjManager::objs[obj.m_head_id].m_x - ObjManager::objs[0].tail; break;
            default:
                break;
            }
        }
        size = obj.m_size;
        r = obj.m_r;
        g = obj.m_g;
        b = obj.m_b;
        
        RECT ObjRect = { x-size, y-size, x+size, y+size };
        HBRUSH ObjBrush = CreateSolidBrush(RGB(r, g, b));
        FillRect(m_hdc, &ObjRect, ObjBrush);
        DeleteObject(ObjBrush);
    }
}
