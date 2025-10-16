#pragma once
#include <windows.h>
#include "ObjManager.h"

class Painter
{
    HDC m_hdc = {};
    int m_map_size = 700;
public:
    Painter() {
        m_hdc = {};
        m_map_size = 700;
    };
    Painter(HDC hdc) { m_hdc = hdc; };
    void Draw(HDC hdc);
private:
    void DrawBackGround();
    void DrawObject();
};

