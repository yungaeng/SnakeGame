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
void ObjManager::MoveSnake(int id, int target_x, int target_y)
{
    // 뱀의 현재 위치 (cx, cy)와 이동 속도 (d)
    double cx = m_snakes[id].body.begin()->m_pos.x; // double 사용을 위해 타입 변경
    double cy = m_snakes[id].body.begin()->m_pos.y; // double 사용을 위해 타입 변경
    double d = m_snakes[id].body.begin()->m_speed; // 이동할 거리 (m_speed)

    double dx = target_x - cx;
    double dy = target_y - cy;
    double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

    if (distance == 0.0) {
        return; 
    }

    double unit_dx = dx / distance;
    double unit_dy = dy / distance;

    double new_x = cx + (unit_dx * d);
    double new_y = cy + (unit_dy * d);

    if (new_y > 10 && new_y < MAP_SIZE - 40 && new_x > 5 && new_x < MAP_SIZE - 10) {

        // 7. 이동 전 현재 위치를 이전 위치로 저장 (이동이 확정된 후에만 실행)
        for (size_t i = 0; i < m_snakes[id].body.size(); i++) {
            m_snakes[id].body[i].m_pos.prev_x = m_snakes[id].body[i].m_pos.x;
            m_snakes[id].body[i].m_pos.prev_y = m_snakes[id].body[i].m_pos.y;
        }

        // 8. 뱀의 머리를 'm_speed'만큼 이동시킨 새 위치로 업데이트
        m_snakes[id].body.begin()->m_pos.x = new_x;
        m_snakes[id].body.begin()->m_pos.y = new_y; 

        // 9. 몸통 따라오기
        if (m_snakes[id].body.size() > 1) {
            for (size_t i = 1; i < m_snakes[id].body.size(); i++) {
                m_snakes[id].body[i].m_pos.x = m_snakes[id].body[i - 1].m_pos.prev_x;
                m_snakes[id].body[i].m_pos.y = m_snakes[id].body[i - 1].m_pos.prev_y;
            }
        }
    }
}
void ObjManager::SnakeEatFood(int id)
{
    size_t tailIndex = m_snakes[id].body.size() - 1;

    int new_x = m_snakes[id].body[tailIndex].m_pos.prev_x;
    int new_y = m_snakes[id].body[tailIndex].m_pos.prev_y;

    pos p = { new_x, new_y, 0, 0 };
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
    ReorderIterater();
}
void ObjManager::FoodCollisions()
{
    auto snake = m_snakes[0].body.begin();
    for (int f=0;f<m_foods.size(); f++)
    {
        if (snake->CheckCollision(m_foods[f]))
        {
            m_foods[f].isalive = false;
            SnakeEatFood(0);
        } 
    }
}

int ObjManager::SnakeCollisions()
{
    return -1;
}

void ObjManager::ReorderIterater()
{
    for (int f = 0; f < m_foods.size(); f++)
    {
        if (m_foods[f].isalive == false)
            m_foods.erase(m_foods.begin() + f);
    }
}
