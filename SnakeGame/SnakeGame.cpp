#pragma once
#include "Game.h"
#include "resource.h"
#pragma comment(lib, "winmm.lib")
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

PAINTSTRUCT ps;
HDC hdc;
Game g_game;
HDC g_hMemDC = NULL;
HBITMAP g_hBitmap = NULL;
HBITMAP g_hOldBitmap = NULL;

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

    // ���� ���� �ؽ�Ʈ
    const wchar_t* descriptionText =
        L"���� ����\n"
        L"------------------------------------------\n"
        L"���� Ű: �����¿� �̵�\n"
        L"Q Ű: ���� ����\n"
        L"���� ���� ����: �Ӹ��� ���� �Ǵ� Ÿ���� ���뿡 �ε��� ���\n"
        L"------------------------------------------";

    static UserData* pSettings = &g_game.userdata;
    switch (message) {
    case WM_INITDIALOG: {
        // DialogBoxParam���� ���޵� lParam�� GameSettings ����ü �����Ϳ� ����
        pSettings = (UserData*)lParam;

        // RGB �Է¶��� �⺻�� ���� (��: 255, 255, 255)
        SetDlgItemText(hDlg, IDC_R_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_G_EDIT, L"255");
        SetDlgItemText(hDlg, IDC_B_EDIT, L"255");

        SetDlgItemText(hDlg, IDC_DESCRIPTION_STATIC, descriptionText);

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
            GetDlgItemText(hDlg, IDC_NICKNAME_EDIT, pSettings->name, MAX_NAME_SIZE);
            if (pSettings->name[0] == L'\0') {
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
            pSettings->color = RGB(r,g,b);
            EndDialog(hDlg, IDOK); // IDOK�� ����
            return TRUE;
        }
        case IDCANCEL: {
            EndDialog(hDlg, IDCANCEL); // IDCANCEL�� ����
            return FALSE;
        }
        }
        break;
    }
    }


    // ������ ��ư�� ������, �α��� ��Ŷ�� ���� ��, ���.
    // ������ GameSetting�� �����ָ� ���� �����Ϳ� �ݿ� �� ���� ����. 
    return FALSE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) 
    {
    case WM_CREATE: 
    {
        g_game.InitGame(hdc);

        HDC hdc = GetDC(hwnd);
        g_hMemDC = CreateCompatibleDC(hdc);
        g_hBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
        g_hOldBitmap = (HBITMAP)SelectObject(g_hMemDC, g_hBitmap);
        ReleaseDC(hwnd, hdc);
        return 0;
    }
    case WM_PAINT: 
    {
        hdc = BeginPaint(hwnd, &ps);
        RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        FillRect(g_hMemDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

        g_game.Draw(g_hMemDC);

        BitBlt(hdc,0, 0,WINDOW_WIDTH, WINDOW_HEIGHT,g_hMemDC,0, 0,SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN: 
    case WM_MOUSEMOVE:
        g_game.Input(wParam, lParam);
        InvalidateRect(hwnd, NULL, false);
        break;
    case WM_DESTROY:
        SelectObject(g_hMemDC, g_hOldBitmap);
        DeleteObject(g_hBitmap);
        DeleteDC(g_hMemDC);
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // ������ ����
    g_game.InitNetwork();

    // ���̾�α� �ڽ� ǥ�� �� ������ �Է�
    INT_PTR dialogResult = DialogBoxParam(
        hInstance,
        MAKEINTRESOURCE(IDD_START_DIALOG),
        NULL,
        StartDialogProc,
        (LPARAM)&g_game.userdata);

    // IDOK�� �ƴϸ� (IDCANCEL �Ǵ� ����) ���α׷� ����
    if (dialogResult != IDOK) {
        return 0;
    }

    // �ɳ�Ʈ ��->
    // �α��� ���� ������ ������
    g_game.Send();


    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SnakeGame";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return -1;
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

    g_game.StartBGM();
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
            g_game.Update();
            
            // ���� ���� üũ �� ���� ó��
            if (g_game.m_isgameover) {
                int killer_id = g_game.m_killer_id;

                wchar_t message_buffer[256];
                wchar_t* name = g_game.GetNameById(killer_id);
                swprintf_s(message_buffer,
                    256,
                    L"����� %s ���� �׾����ϴ�!! �ٽ��Ͻðڽ��ϱ�?",
                    name);
                int result = MessageBox(hwnd,
                    message_buffer,
                    L"Game Over",
                    MB_YESNO | MB_ICONQUESTION);

                if (result == IDYES) {
                    // '��'�� ������ ���: 
                    g_game.ReStart(); // TODO : ����� ����
                    continue;
                }
                else {
                    // ����ڰ� '�ƴϿ�' (����)�� ������ ��
                    g_game.EndNetwork();       // ��Ʈ��ũ ����
                    PostQuitMessage(0);        // ���� ����
                }
                continue;

            }
            // ������Ʈ �� ȭ�� ��ȿȭ.
            InvalidateRect(hwnd, NULL, false);
        }
    }
    g_game.StopBGM();
    // WM_QUIT �޽����� wParam ���� ��ȯ
    return (int)msg.wParam;
}


