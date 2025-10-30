#include "ObjManager.h"

std::vector<Object> ObjManager::m_foods;
std::vector<Snake> ObjManager::m_snakes;

void ObjManager::AddFood(int x, int y, COLORREF c)
{
    m_foods.emplace_back(Object(x, y, c));
}
void ObjManager::AddSnake(UserData ud)
{
    int x = 350;
    int y = rand() % 700;
    std::vector<Object> new_snake;
    new_snake.emplace_back(Object(x, y, ud.color));
    Snake s = { ud.name, new_snake };
    m_snakes.emplace_back(s);
}
void ObjManager::MoveSnake(int id, int target_x, int target_y)
{
    // 뱀의 현재 위치 (cx, cy)와 이동 속도 (d)
    double cx = m_snakes[id].body.begin()->m_x; // double 사용을 위해 타입 변경
    double cy = m_snakes[id].body.begin()->m_y; // double 사용을 위해 타입 변경
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
        if (m_snakes[id].body.size() > 1) {
            for (size_t i = m_snakes[id].body.size() - 1; i > 0; i--) {
                // i 번째 조각의 현재 위치는 바로 앞 조각 (i-1)의 현재 위치를 따라갑니다.
                m_snakes[id].body[i].m_x = m_snakes[id].body[i - 1].m_x;
                m_snakes[id].body[i].m_y = m_snakes[id].body[i - 1].m_y;
            }
        }

        // 10. 뱀의 머리를 'm_speed'만큼 이동시킨 새 위치로 업데이트 (몸통 이동 후에 실행)
        m_snakes[id].body.front().m_x = new_x;
        m_snakes[id].body.front().m_y = new_y;
    }
}
void ObjManager::DeleteSnake(int id)
{
    m_snakes.erase(m_snakes.begin() + id);
}

void ObjManager::SnakeEatFood(int id)
{
    auto& body = m_snakes[id].body;
    size_t size = body.size();

    if (size < 2) {
        body.emplace_back(Object(body.front().m_x, body.front().m_y, body.front().m_color));
        return;
    }

    const auto& tail = body[size - 1];
    const auto& prev_tail = body[size - 2];
    const double SEGMENT_SIZE = tail.m_size;

    double dx = prev_tail.m_x - tail.m_x;
    double dy = prev_tail.m_y - tail.m_y;
    double distance = std::sqrt(dx * dx + dy * dy);

    double unit_dx = (distance != 0.0) ? dx / distance : 0.0;
    double unit_dy = (distance != 0.0) ? dy / distance : 0.0;


    double new_x = tail.m_x - (unit_dx * SEGMENT_SIZE); // 꼬리->앞꼬리 벡터를 뺌
    double new_y = tail.m_y - (unit_dy * SEGMENT_SIZE); // 꼬리->앞꼬리 벡터를 뺌

    COLORREF c = body.front().m_color;
    body.emplace_back(Object(new_x, new_y, c));
}
bool ObjManager::UpDate()
{
    HandleCollisions();
    GarbageCollector();
    return gameover;
}
void ObjManager::HandleCollisions()
{
    FoodCollisions();
    DeathBy = SnakeCollisions();
}
void ObjManager::FoodCollisions()
{
    auto& snake_head = m_snakes[0].body.front(); // 참조를 사용하고 front() 사용
    for (int f = (int)m_foods.size() - 1; f >= 0; f--)
    {
        if (snake_head.CheckCollision(m_foods[f]))
        {
            m_foods[f].isalive = false;
            SnakeEatFood(0);
        }
    }
}
int ObjManager::SnakeCollisions()
{
    Snake my_snake = m_snakes[0];
    for (int i = 2; i < my_snake.body.size(); i++)
    {
        if (my_snake.body[i].CheckCollision(my_snake.body.front()))
        {
            gameover = true;
            return 0;
        }
    }

    for (int other = 1; other < m_snakes.size(); other++) {
        for (int i = 2; i < my_snake.body.size(); i++)
        {
            if (m_snakes[other].body.begin()->CheckCollision(my_snake.body[i]))
            {
                gameover = true;
                return other;
            }
        }
    }
    return -1;
}
void ObjManager::GarbageCollector()
{
    m_foods.erase(std::remove_if(m_foods.begin(), m_foods.end(),
            [](const Object& food) {
                return food.isalive == false; // isalive가 false인 요소를 삭제
            }),
        m_foods.end());
}
