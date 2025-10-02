#pragma once
#include <windows.h>
#include "Painter.h"
#include "ObjManager.h"

PAINTSTRUCT ps;
HDC hdc;
ObjManager objmanager;
Painter painter(hdc);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        for(int i = 0; i<10; ++i) 
            objmanager.AddObjRandom();
        break;
    }
    case WM_PAINT: {
        hdc = BeginPaint(hwnd, &ps);

        painter.Draw(hdc);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN: {
        switch (wParam) {
        case VK_UP:
            objmanager.MoveObj(0, 0);
            break;
        case VK_DOWN:
            objmanager.MoveObj(0, 1);
            break;
        case VK_LEFT:
            objmanager.MoveObj(0, 2);
            break;
        case VK_RIGHT:
            objmanager.MoveObj(0, 3);
            break;
        case 'Q':
            PostQuitMessage(0);
            return 0;
        }

        InvalidateRect(hwnd, NULL, false);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    objmanager.UpdateObj();

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


