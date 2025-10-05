#include "ObjManager.h"

std::vector<Object> ObjManager::m_foods;
std::vector<Object> ObjManager::m_snake;

ObjManager::ObjManager()
{
    // ������ ���� ������ �� �ʱ�ȭ�� ����� ������
    // ���⼭ ���� �ʱ�ȭ �۾��� ������ �ΰڴ�

    // 1. ���� ����
    for (int i = 0; i < 20; i++)
    {
        pos p = { rand() % 700,rand() % 700,rand() % 700 };
        color c = { rand() % 255, rand() % 255,rand() % 255 };
        m_foods.emplace_back(Object(p, c));
    }

    // 2. �� ����
    AddSnake();
}

ObjManager::~ObjManager()
{
}

void ObjManager::AddFood(pos p, color c)
{
    m_foods.emplace_back(Object(p, c));
}


void ObjManager::AddSnake()
{
    for (int i = 0; i < 10; i++) {
        pos p = { rand() % 700,rand() % 700,rand() % 700 };
        color c = { rand() % 255, rand() % 255,rand() % 255 };
        m_snake.emplace_back(Object(p,c));
    }
}

void ObjManager::MoveSnake(dir d)
{
    int x = m_snake[0].m_pos.x, y = m_snake[0].m_pos.y;

    switch (d)
    {
    case UP: y -= m_snake[0].m_speed; break;
    case DOWN:  y += m_snake[0].m_speed; break;
    case LEFT: x -= m_snake[0].m_speed; break;
    case RIGHT: x += m_snake[0].m_speed; break;
    default:
        break;
    }

    for (int i = 0; i < m_snake.size(); i++) {
        m_snake[i].m_pos.prev_x = m_snake[i].m_pos.x;
        m_snake[i].m_pos.prev_y = m_snake[i].m_pos.y;
    }

    m_snake[0].m_pos.x = x;
    m_snake[0].m_pos.y = y;

    if (m_snake.size() > 1) {
        for (int i = 1; i < m_snake.size(); i++) {
            m_snake[i].m_pos.x = m_snake[i-1].m_pos.prev_x;
            m_snake[i].m_pos.y = m_snake[i-1].m_pos.prev_y;
        }
    }
}

void ObjManager::SnakeEatFood()
{
    int tail = m_snake.size() - 1;
    int x = m_snake[tail].m_pos.x;
    int y = m_snake[tail].m_pos.y;

    switch (m_snake[0].m_dir)
    {
    case 0: y -= 20; break;
    case 1:  y += 20; break;
    case 2: x -= 20; break;
    case 3: x += 20; break;
    default:
        break;
    }
    pos p = { x,y,x,y };
    color c = { rand() % 255, rand() % 255,rand() % 255 };
    m_snake.emplace_back(Object(p, c));
}

void ObjManager::UpDate(bool* gamestate)
{
    // ������Ʈ ���� �ؾ� �ϴ� ��
    // 1. ������Ʈ ������ ��������
    // 
    // 2. ������ ������ ���� �����Ϳ� ����ȭ ó��
    // 
    // 3. ������ ����ȭ�� ��� ������Ʈ �浹ó��
    HandleCollisions();
    // 4. ����� ��� ������ �۽�ó��
    //
    if (m_isgameover)
        *gamestate = true;
}

void ObjManager::HandleCollisions()
{
    // ��� ������ �浹üũ
    for (size_t i = 0; i < m_foods.size(); ++i) {
        // 1. �� 0���� ������ �浹üũ
        if (m_snake[0].CheckCollision(m_foods[i])) {
            // ���⿡ �浹 ó�� �ۼ�
            m_foods.erase(m_foods.begin() + i);
            SnakeEatFood();
        }
    }

    // ��� ���� ���� �浹üũ
    for (size_t i = 1; i < m_snake.size(); ++i) {
        // 1. �� 0���� �Ӹ��� ������ �浹üũ
        if (m_snake[0].CheckCollision(m_snake[i])) {
            // ���⿡ �浹 ó�� �ۼ�
            // ���� ���� flag, update���� ���� ����޽��� ����
            m_isgameover = true;
        }
    }
}