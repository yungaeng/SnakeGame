#pragma once
#include <windows.h>
#include "Painter.h"
#include "ObjManager.h"
#include "Game.h"
<<<<<<< HEAD
#include "resource.h"
#include <mmsystem.h>
#include "gamedata.h"
#pragma comment(lib, "winmm.lib")
=======
>>>>>>> parent of d77d0bf (music 2)

PAINTSTRUCT ps;
HDC hdc;
Game g_game;

<<<<<<< HEAD
void PlayBGM() {
    // S_ASYNC: �񵿱������� ��� (���� ������ ��� ����)
    // SND_LOOP: ���� �ݺ� ���
    // SND_FILENAME: ���� �̸����� ���
    // TODO: "bgm.wav" ������ ������Ʈ ���� �Ǵ� ���� ���� ��ο� �ξ�� �մϴ�.
    PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}
void StopBGM() {
    PlaySound(NULL, NULL, 0);
}

struct GameSettings {
    WCHAR szNickName[256];
    COLORREF playerColor;
    bool isGameStarted; // ���� ���� ����
} g_settings;

// ���� �Լ�: Edit Control���� �ؽ�Ʈ�� ������ ������ ��ȯ�ϰ� 0~255 ���� Ȯ��
int GetIntFromEdit(HWND hDlg, int nIDDlgItem) {
    WCHAR szText[5]; // �ִ� "255\0"
    GetDlgItemText(hDlg, nIDDlgItem, szText, 5);
    int value = _wtoi(szText);
    if (value < 0 || value > 255) {
        return -1; // ��ȿ���� ���� ��
    }
    return value;
}

INT_PTR CALLBACK StartDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // ���̾�α׿� ������ GameSettings ����ü ������
    static GameSettings* pSettings = nullptr;

    switch (message) {
    case WM_INITDIALOG: {
        // DialogBoxParam���� ���޵� lParam�� GameSettings ����ü �����Ϳ� ����
        pSettings = (GameSettings*)lParam;

        // RGB �Է¶��� �⺻�� ���� (��: 255, 255, 255)
        SetDlgItemText(hDlg, IDC_R_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_G_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_B_EDIT, L"255");

        RECT rcDlg;
        // 1. ���� ���̾�α� â�� ũ�⸦ ����ϴ�. (ȭ�� ��ǥ ����)
        GetWindowRect(hDlg, &rcDlg);

        // 2. ���̾�α��� ���� ���̸� ����մϴ�.
        int nWidth = rcDlg.right - rcDlg.left;
        int nHeight = rcDlg.bottom - rcDlg.top;

        // 3. �� ������� ȭ�� �ػ󵵸� ����ϴ�.
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        // 4. ���̾�αװ� ȭ�� �߾ӿ� ��ġ�� ��ǥ�� ����մϴ�.
        int nX = (nScreenWidth - nWidth) / 2;
        int nY = (nScreenHeight - nHeight) / 2;

        // 5. ���̾�α� â�� ��ġ�� �����մϴ�.
        SetWindowPos(hDlg,
            HWND_TOP,
            nX,
            nY,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER);

        // ��Ŀ���� �������� ������ TRUE ��ȯ, ��Ŀ���� ���� �����ϸ� FALSE ��ȯ
        return TRUE;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK: {
            if (!pSettings) break;

            // 1. �г��� �������� �� Ȯ��
            GetDlgItemText(hDlg, IDC_NICKNAME_EDIT, pSettings->szNickName, 256);
            if (pSettings->szNickName[0] == L'\0') {
                MessageBox(hDlg, L"�г����� �Է����ּ���.", L"�Է� ����", MB_ICONWARNING);
                SetFocus(GetDlgItem(hDlg, IDC_NICKNAME_EDIT));
                return TRUE;
            }

            // 2. RGB �� �������� �� Ȯ�� (0~255 ���� üũ)
            int r = GetIntFromEdit(hDlg, IDC_R_EDIT);
            int g = GetIntFromEdit(hDlg, IDC_G_EDIT);
            int b = GetIntFromEdit(hDlg, IDC_B_EDIT);

            if (r == -1 || g == -1 || b == -1) {
                MessageBox(hDlg, L"RGB ���� 0���� 255 ������ �������� �մϴ�.", L"�Է� ����", MB_ICONWARNING);
                // ������ �߻��� ��Ʈ�ѿ� ��Ŀ�� ���� (�ʿ��)
                return TRUE;
            }

            // 3. GameSettings�� ���� �� ���̾�α� ����
            pSettings->playerColor = RGB(r, g, b);
            pSettings->isGameStarted = true;
            EndDialog(hDlg, IDOK); // IDOK�� ����
            return TRUE;
        }
        case IDCANCEL: {
            if (pSettings) {
                pSettings->isGameStarted = false; // ���� ��ư Ŭ�� �� ���� ���� �� ��
            }
            EndDialog(hDlg, IDCANCEL); // IDCANCEL�� ����
            return TRUE;
        }
        }
        break;
    }
    }
    return FALSE;
}

=======
>>>>>>> parent of d77d0bf (music 2)
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

<<<<<<< HEAD
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. ���̾�α� �ڽ� ǥ�� �� ������ �Է�
    // DialogBoxParam�� ����Ͽ� g_settings�� �����͸� ���̾�α� ���ν����� ����
    INT_PTR dialogResult = DialogBoxParam(
        hInstance,
        MAKEINTRESOURCE(IDD_START_DIALOG), // resource.h�� ���ǵ� ID
        NULL, // ���� ������ ���� ���̹Ƿ� NULL
        StartDialogProc,
        (LPARAM)&g_settings
    );
=======
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };    
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SnakeGame";
>>>>>>> parent of d77d0bf (music 2)

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

<<<<<<< HEAD
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SnakeGame";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return -1;
=======
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
            // recv
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
>>>>>>> parent of d77d0bf (music 2)
    }
    // WM_QUIT �޽����� wParam ���� ��ȯ
    return (int)msg.wParam;
}

    // ȭ�� �߾� ��ǥ ���
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    int x_pos = (screen_width - WINDOW_WIDTH) / 2;
    int y_pos = (screen_height - WINDOW_HEIGHT) / 2;

    // ������ ����
    HWND hwnd = CreateWindow(L"SnakeGame", L"SnakeGame",
        WS_OVERLAPPEDWINDOW,
        x_pos,           // X ��ǥ (ȭ�� �߾�)
        y_pos,           // Y ��ǥ (ȭ�� �߾�)
        WINDOW_WIDTH,    // ��
        WINDOW_HEIGHT,   // ����
        NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, L"Failed to create window!", L"Error", MB_ICONERROR);
        return -1;
    }

    PlayBGM();
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
            // recv
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
                    g_game.ReStart();

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

    StopBGM();

    // WM_QUIT �޽����� wParam ���� ��ȯ
    return (int)msg.wParam;
}


