#pragma once
#define MAP_SIZE 700
#define MAX_NAME_SIZE 10

// â�� ���ϴ� ũ�� (CreateWindow�� 5, 6��° ���ڿ� ��ġ�ؾ� ��)
const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 700;

struct UserData {
    wchar_t name[MAX_NAME_SIZE];
    int r;
    int g;
    int b;
};

struct Snake {
    wchar_t name[MAX_NAME_SIZE];
    std::vector<Object> body;

    Snake(const wchar_t* n, std::vector<Object> b) { wcscpy_s(name, MAX_NAME_SIZE, n); body = b; };
};