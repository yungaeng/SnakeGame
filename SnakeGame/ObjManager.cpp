#include "ObjManager.h"
#include <atomic>

std::unordered_map<int, Object> ObjManager::objs;
std::vector<Object> ObjManager::snake;

void ObjManager::AddObj(int id, int x, int y, int size, int r, int g, int b)
{
	Object o(id, x, y, size, r, g, b);
    std::pair<int, Object> p(id, o);
	objs.insert(p);
}

void ObjManager::AddObjRandom(int id)
{
	Object o(id, rand() % 700, rand() % 700, 10, rand() % 255, rand() % 255, rand() % 255);
    std::pair<int, Object> p(id, o);
    objs.insert(p);
}

void ObjManager::MoveObj(int index, char dir) {

    objs[index].m_prev_x = objs[index].m_x;
    objs[index].m_prev_y = objs[index].m_y;

    switch (dir)
    {
    case 0: objs[index].m_y -= objs[index].m_speed; break;
    case 1:  objs[index].m_y += objs[index].m_speed; break;
    case 2: objs[index].m_x -= objs[index].m_speed; break;
    case 3: objs[index].m_x += objs[index].m_speed; break;
    default:
        break;
    }

    objs[index].m_dir = dir;

    for (size_t i = 0; i < objs.size(); ++i) {
        if (objs[i].m_target != -1) {
            int t_id = objs[i].m_target;
            
            objs[i].m_x = objs[t_id].m_prev_x;
            objs[i].m_y = objs[t_id].m_prev_y;
        }
    }
}

void ObjManager::UpdateObj()
{
    HandleCollisions();
}

void ObjManager::AddSnake()
{
    for (int i = 0; i < 10; i++) {
        Object o(i, rand() % 700, rand() % 700, 10, rand() % 255, rand() % 255, rand() % 255);
        snake.emplace_back(o);
    }
}

void ObjManager::MoveSnake(char dir)
{
    int x = snake[0].m_x, y = snake[0].m_y;

    switch (dir)
    {
    case 0: y -= snake[0].m_speed; break;
    case 1:  y += snake[0].m_speed; break;
    case 2: x -= snake[0].m_speed; break;
    case 3: x += snake[0].m_speed; break;
    default:
        break;
    }

    for (int i = 0; i < snake.size(); i++) {
        snake[i].m_prev_x = snake[i].m_x;
        snake[i].m_prev_y = snake[i].m_y;
    }

    snake[0].m_x = x;
    snake[0].m_y = y;

    if (snake.size() > 1) {
        for (int i = 1; i < snake.size(); i++) {
            snake[i].m_x = snake[i-1].m_prev_x;
            snake[i].m_y = snake[i-1].m_prev_y;
        }
    }
}

void ObjManager::SnakeEatFood()
{
    int tail = snake.size();
    int x = snake[tail - 1].m_x;
    int y = snake[tail - 1].m_y;

    switch (snake[0].m_dir)
    {
    case 0: y -= 20; break;
    case 1:  y += 20; break;
    case 2: x -= 20; break;
    case 3: x += 20; break;
    default:
        break;
    }
    Object o(tail, x, y, 10, rand() % 255, rand() % 255, rand() % 255);
    snake.emplace_back(o);
}

std::atomic<int> tail;

void ObjManager::HandleCollisions()
{
    for (size_t i = 0; i < objs.size(); ++i) {
        if (snake[0].CheckCollision(objs[i])) {
            // 여기에 충돌 처리 작성
            objs.erase(i);
            SnakeEatFood();
        }
    }
}

