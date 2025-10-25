#pragma once
#include <windows.h>
#include "ObjManager.h"
#include <sstream>

class Painter
{
    int m_map_size = MAP_SIZE;
public:
    Painter() {
        m_map_size = MAP_SIZE;
    };
    void Draw(HDC hdc);
private:
    void DrawBackGround(HDC hdc);
    void DrawObject(HDC hdc);
    void DrawName(HDC hdc, int x, int y, wchar_t name[MAX_NAME_SIZE], int size);
};

