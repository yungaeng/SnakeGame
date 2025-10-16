#pragma once
#include <windows.h>
#include "Painter.h"
#include "ObjManager.h"
#include "Game.h"

PAINTSTRUCT ps;
HDC hdc;
ObjManager objmanager;
Painter painter(hdc);
bool isgameover = false;
Game g_game;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) 
    {
    case WM_CREATE: 
    {
        objmanager.AddSnake();

        for (int i = 0; i < 10; i++)
        {
            pos p = { rand() % 600, rand() % 600 };
            color c = { rand() % 255, rand() % 255,rand() % 255 };
            objmanager.AddFood(p, c);
        }
       
        break;
    }
    case WM_PAINT: 
    {
        hdc = BeginPaint(hwnd, &ps);
        painter.Draw(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN: 
    {
        switch (wParam) 
        {
        case VK_UP: objmanager.MoveSnake(UP); break;
        case VK_DOWN: objmanager.MoveSnake(DOWN); break;
        case VK_LEFT: objmanager.MoveSnake(LEFT); break;
        case VK_RIGHT: objmanager.MoveSnake(RIGHT); break;

        case 'W': objmanager.MoveOtherSnake(UP); break;
        case 'S': objmanager.MoveOtherSnake(DOWN); break;
        case 'A': objmanager.MoveOtherSnake(LEFT); break;
        case 'D': objmanager.MoveOtherSnake(RIGHT); break;

        case 'Q': PostQuitMessage(0); return 0;
        }
        InvalidateRect(hwnd, NULL, false);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0); return 0;
    default:
    {
        // IDLE일 때 업데이트
        objmanager.UpDate(&isgameover);
        // gameover check
        if(isgameover == true)
            PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SnakeGame";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return -1;
    }

    HWND hwnd = CreateWindow(L"SnakeGame", L"SnakeGame",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
        700, 700, NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, L"Failed to create window!", L"Error", MB_ICONERROR);
        return -1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


