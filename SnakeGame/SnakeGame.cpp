#pragma once
#include <windows.h>
#include "Painter.h"
#include "ObjManager.h"
#include "Game.h"

PAINTSTRUCT ps;
HDC hdc;
Game g_game;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) 
    {
    case WM_CREATE: 
    {
        g_game.InitGame(hdc);
        break;
    }
    case WM_PAINT: 
    {
        hdc = BeginPaint(hwnd, &ps);
        g_game.Draw(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN: 
    {
        g_game.InputKey(wParam);
        InvalidateRect(hwnd, NULL, false);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0); return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
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

    while (true) {
        // 1. �޽����� �ִ��� Ȯ���ϰ� ������ ó�� (Non-blocking)
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // WM_QUIT �޽����� ������ ������ ����
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // �޽����� ���� �� (Idle Time)
            // ������Ʈ�� fps �߰��ؾ� ��
            g_game.UpdateGame();

            // ���� ���� üũ �� ���� ó��
            if (g_game.IsGameOver()) {
                int result = MessageBox(hwnd,
                    L"Game Over! Do you want to restart the game?", // ���� ����
                    L"Game Over",
                    MB_YESNO | MB_ICONQUESTION); // ��/�ƴϿ� ��ư ���

                if (result == IDYES) {
                    // '��'�� ������ ���: 
                    //  TODO : ������ ��.
                    continue; // ���� ������ �̵��Ͽ� ���ο� ���� ����
                }
                else {
                    // ����ڰ� '�ƴϿ�' (����)�� ������ ��
                    PostQuitMessage(0); // ���� ����
                }
                continue;
            }
        }
    }
    // WM_QUIT �޽����� wParam ���� ��ȯ
    return (int)msg.wParam;
}


