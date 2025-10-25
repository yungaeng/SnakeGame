#include "ObjManager.h"

std::vector<Object> ObjManager::m_foods;
std::vector<Snake> ObjManager::m_snakes;

void ObjManager::AddFood(pos p, COLORREF c)
{
    m_foods.emplace_back(Object(p, c));
}

void ObjManager::AddSnake(UserData ud)
{
    pos p{ 350, rand() % 700 };
    std::vector<Object> new_snake;
    new_snake.emplace_back(Object(p, ud.color));
    Snake s = { ud.name, new_snake };
    m_snakes.emplace_back(s);
}
void ObjManager::MoveSnake(int id, dir d)
{
    int x = m_snakes[id].body.begin()->m_pos.x, y = m_snakes[id].body.begin()->m_pos.y;

    switch (d)
    {
    case UP: y -= m_snakes[id].body.begin()->m_speed; break;
    case DOWN: y += m_snakes[id].body.begin()->m_speed; break;
    case LEFT: x -= m_snakes[id].body.begin()->m_speed; break;
    case RIGHT: x += m_snakes[id].body.begin()->m_speed; break;
    default:
        break;
    }

    if (y > 10 && y < MAP_SIZE - 40 && x > 5 && x < MAP_SIZE - 10) {
        for (int i = 0; i < m_snakes[id].body.size(); i++) {
            m_snakes[id].body[i].m_pos.prev_x = m_snakes[id].body[i].m_pos.x;
            m_snakes[id].body[i].m_pos.prev_y = m_snakes[id].body[i].m_pos.y;
        }

        m_snakes[id].body.begin()->m_pos.x = x;
        m_snakes[id].body.begin()->m_pos.y = y;

        if (m_snakes[id].body.size() > 1) {
            for (int i = 1; i < m_snakes[id].body.size(); i++) {
                m_snakes[id].body[i].m_pos.x = m_snakes[id].body[i - 1].m_pos.prev_x;
                m_snakes[id].body[i].m_pos.y = m_snakes[id].body[i - 1].m_pos.prev_y;
            }
        }
    }
}
void ObjManager::SnakeEatFood(int id)
{
    int tail = m_snakes[id].body.end();
    int x = m_snakes[id].body[tail].m_pos.x;
    int y = m_snakes[id].body[tail].m_pos.y;

    switch (m_snakes[id].body.begin()->m_dir)
    {
    case 0: y -= 20; break;
    case 1:  y += 20; break;
    case 2: x -= 20; break;
    case 3: x += 20; break;
    default:
        break;
    }
    pos p = { x,y,0,0 };
    COLORREF c = m_snakes[id].body.begin()->m_color;
    m_snakes[id].body.emplace_back(Object(p, c));
}
void ObjManager::DeleteSnake(int id)
{
    m_snakes.erase(m_snakes.begin() + id);
}

bool ObjManager::UpDate()
{
    HandleCollisions();
    return gameover;
}

void ObjManager::HandleCollisions()
{
    FoodCollisions();
    DeathBy = SnakeCollisions();
}
void ObjManager::FoodCollisions()
{
    // 모든 뱀
    for (int id = 0; id < m_snakes.size(); ++id) {
        auto& snake_head = *m_snakes[id].body.begin();

        auto it = m_foods.begin();
        while (it != m_foods.end()) {
            // 뱀의 머리와 현재 먹이(*it) 충돌 체크
            if (snake_head.CheckCollision(*it)) {
                SnakeEatFood(id);
                it = m_foods.erase(it);
                return;
            }
            else {
                ++it;
            }
        }
    }
}

int ObjManager::SnakeCollisions()
{
    // 모든 뱀들의 몸통 충돌체크
    for (int id = 0; id < m_snakes.size(); id++) {
        for (int i = 1; i < m_snakes[id].body.size(); i++) {
            if (m_snakes[id].body.begin()->CheckCollision(m_snakes[id].body[i])) {
                gameover = true;
                return i;
            }
        }
    }

    return -1; // 충돌 없음
}
