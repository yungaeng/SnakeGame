#pragma once
#include <windows.h>
#include "ObjManager.h"

class Painter
{
    HDC m_hdc;
    int map_size = 700;
    
public:
    Painter(HDC hdc) { m_hdc = hdc; };
    void Draw(HDC hdc);
private:
    void DrawBackGround();
    void DrawObject();
};

