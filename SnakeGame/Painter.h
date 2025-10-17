#pragma once
#include <windows.h>
#include "ObjManager.h"
#include "gamedata.h"

class Painter
{
    HDC m_hdc = {};
    int m_map_size = 700;
public:
    Painter() {
        m_hdc = {};
        m_map_size = MAP_SIZE;
    };
    Painter(HDC hdc) { m_hdc = hdc; };
    void Draw(HDC hdc);
    void SetHDC(HDC hdc) { m_hdc = hdc; };
private:
    void DrawBackGround();
    void DrawObject();
};

