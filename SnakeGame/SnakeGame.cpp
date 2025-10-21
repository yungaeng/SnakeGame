#pragma once
#include <windows.h>
#include "Game.h"
#include "resource.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

PAINTSTRUCT ps;
HDC hdc;
Game g_game;

void PlayBGM() {
    // S_ASYNC: 비동기적으로 재생 (메인 스레드 블록 방지)
    // SND_LOOP: 무한 반복 재생
    // SND_FILENAME: 파일 이름으로 재생
    // TODO: "bgm.wav" 파일을 프로젝트 폴더 또는 실행 파일 경로에 두어야 합니다.
    PlaySound(L"bgm.wav", NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);
}

// 배경음악을 중지하는 함수
void StopBGM() {
    PlaySound(NULL, NULL, 0);
}

// 게임 설정 정보를 담을 구조체
struct GameSettings {
    WCHAR szNickName[256];
    COLORREF playerColor;
    bool isGameStarted; // 게임 시작 여부
} g_settings;

// 헬퍼 함수: Edit Control에서 텍스트를 가져와 정수로 변환하고 0~255 범위 확인
int GetIntFromEdit(HWND hDlg, int nIDDlgItem) {
    WCHAR szText[5]; // 최대 "255\0"
    GetDlgItemText(hDlg, nIDDlgItem, szText, 5);
    int value = _wtoi(szText);
    if (value < 0 || value > 255) {
        return -1; // 유효하지 않은 값
    }
    return value;
}

INT_PTR CALLBACK StartDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // 다이얼로그와 공유할 GameSettings 구조체 포인터
    static GameSettings* pSettings = nullptr;

    switch (message) {
    case WM_INITDIALOG: {
        // DialogBoxParam으로 전달된 lParam을 GameSettings 구조체 포인터에 저장
        pSettings = (GameSettings*)lParam;

        // RGB 입력란에 기본값 설정 (예: 255, 255, 255)
        SetDlgItemText(hDlg, IDC_R_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_G_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_B_EDIT, L"255");

        // 포커스를 닉네임 입력란으로 설정
        SetFocus(GetDlgItem(hDlg, IDC_NICKNAME_EDIT));

        return FALSE; // 포커스를 직접 설정했으므로 FALSE 반환
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK: {
            if (!pSettings) break;

            // 1. 닉네임 가져오기 및 확인
            GetDlgItemText(hDlg, IDC_NICKNAME_EDIT, pSettings->szNickName, 256);
            if (pSettings->szNickName[0] == L'\0') {
                MessageBox(hDlg, L"닉네임을 입력해주세요.", L"입력 오류", MB_ICONWARNING);
                SetFocus(GetDlgItem(hDlg, IDC_NICKNAME_EDIT));
                return TRUE;
            }

            // 2. RGB 값 가져오기 및 확인 (0~255 범위 체크)
            int r = GetIntFromEdit(hDlg, IDC_R_EDIT);
            int g = GetIntFromEdit(hDlg, IDC_G_EDIT);
            int b = GetIntFromEdit(hDlg, IDC_B_EDIT);

            if (r == -1 || g == -1 || b == -1) {
                MessageBox(hDlg, L"RGB 값은 0에서 255 사이의 정수여야 합니다.", L"입력 오류", MB_ICONWARNING);
                // 오류가 발생한 컨트롤에 포커스 설정 (필요시)
                return TRUE;
            }

            // 3. GameSettings에 저장 및 다이얼로그 종료
            pSettings->playerColor = RGB(r, g, b);
            pSettings->isGameStarted = true;
            EndDialog(hDlg, IDOK); // IDOK로 종료
            return TRUE;
        }
        case IDCANCEL: {
            if (pSettings) {
                pSettings->isGameStarted = false; // 종료 버튼 클릭 시 게임 시작 안 함
            }
            EndDialog(hDlg, IDCANCEL); // IDCANCEL로 종료
            return TRUE;
        }
        }
        break;
    }
    }
    return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_CREATE:
    {
        g_game.InitGame(hdc, g_settings.szNickName, g_settings.playerColor);
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

<<<<<< < HEAD
    int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. 다이얼로그 박스 표시 및 설정값 입력
    // DialogBoxParam을 사용하여 g_settings의 포인터를 다이얼로그 프로시저에 전달
    INT_PTR dialogResult = DialogBoxParam(
        hInstance,
        MAKEINTRESOURCE(IDD_START_DIALOG), // resource.h에 정의된 ID
        NULL, // 메인 윈도우 생성 전이므로 NULL
        StartDialogProc,
        (LPARAM)&g_settings
    );

    // 2. 다이얼로그 결과 확인
    if (dialogResult == -1) {
        MessageBox(NULL, L"Failed to create dialog!", L"Error", MB_ICONERROR);
        return -1;
    }

    // 3. '종료' 버튼을 눌렀거나 에러로 다이얼로그가 종료된 경우
    if (dialogResult == IDCANCEL || !g_settings.isGameStarted) {
        return 0; // 프로그램 즉시 종료
    }

    WNDCLASS wc = { 0 };
    ====== =
        int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
        WNDCLASS wc = { 0 };
        >>>>>> > daafa40e3ba6c76ba33b4a576463a95eb5e74512
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

        PlayBGM();
        ShowWindow(hwnd, nCmdShow);
        MSG msg;

        while (true) {
            // 1. 메시지가 있는지 확인하고 있으면 처리 (Non-blocking)
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                // WM_QUIT 메시지를 받으면 루프를 종료
                if (msg.message == WM_QUIT) {
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                // 메시지가 없을 때 (Idle Time)
                // 업데이트에 fps 추가해야 함
                // recv
                g_game.UpdateGame();

                // 게임 오버 체크 및 종료 처리
                if (g_game.IsGameOver()) {
                    int result = MessageBox(hwnd,
                        L"Game Over! Do you want to restart the game?", // 문구 변경
                        L"Game Over",
                        MB_YESNO | MB_ICONQUESTION); // 예/아니오 버튼 사용

                    if (result == IDYES) {
                        // '예'를 선택한 경우: 
                        //  TODO : 만들어야 함.
                        g_game.ReStart();

                        continue; // 다음 루프로 이동하여 새로운 게임 시작
                    }
                    else {
                        // 사용자가 '아니오' (종료)을 눌렀을 때
                        PostQuitMessage(0); // 게임 종료
                    }
                    continue;
                }
            }

        }

        StopBGM();

        // WM_QUIT 메시지의 wParam 값을 반환
        return (int)msg.wParam;
    }


