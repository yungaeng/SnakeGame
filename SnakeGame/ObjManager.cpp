#include "ObjManager.h"

std::vector<Object> ObjManager::m_foods;
std::vector<Snake> ObjManager::m_snakes;
std::mutex ObjManager::obj_lock;

std::atomic<int> g_id = 1;

void ObjManager::AddFood(unsigned long long id, int x, int y, COLORREF c)
{
    obj_lock.lock();
    m_foods.emplace_back(Object(id, x, y, c));
    obj_lock.unlock();
}
void ObjManager::AddSnake(unsigned long long id, UserData ud, int x, int y)
{
    std::vector<Object> new_snake;
    new_snake.emplace_back(Object(-1, x, y, ud.color));
    Snake s = { ud.name, id, new_snake};
    m_snakes.emplace_back(s);
}
void ObjManager::MoveSnake(unsigned long long id, double deltaTime)
{
    Snake* s{ nullptr };
    for (auto& sn : m_snakes)
        if (sn.m_id == id)
            s = &sn;

    /*int mx = m_snakes[id].m_target_x;
    int my = m_snakes[id].m_target_y;
    int& x = m_snakes[id].body.front().m_x;
    int& y = m_snakes[id].body.front().m_y;
    double vel = m_snakes[id].body.front().m_speed;*/

    int mx = s->m_target_x;
    int my = s->m_target_y;
    int& x = s->body.front().m_x;
    int& y = s->body.front().m_y;
    double vel = s->body.front().m_speed;

    double dx = (double)mx - x;
    double dy = (double)my - y;
    double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

    if (distance < 1.0) {
        x = mx;
        y = my;
        return;
    }

    double moveDistance = vel * deltaTime;
    double finalMoveDistance = std::min(moveDistance, distance);
    double ratio = finalMoveDistance / distance;

    int next_x = x + (int)std::round(dx * ratio);
    int next_y = y + (int)std::round(dy * ratio);

    if (finalMoveDistance == distance) {
        next_x = mx;
        next_y = my;
    }

    if (next_y > 10 && next_y < MAP_SIZE - 40 &&
        next_x > 5 && next_x < MAP_SIZE - 10) 
    {
        if (s->body.size() > 1) {
            for (size_t i = s->body.size() - 1; i > 0; i--) {
                const double SEGMENT_SIZE = s->body.front().m_size;

                double prev_x = s->body[i - 1].m_x;
                double prev_y = s->body[i - 1].m_y;
                double current_x = s->body[i].m_x;
                double current_y = s->body[i].m_y;

                double vec_x = current_x - prev_x;
                double vec_y = current_y - prev_y;
                double dist = std::sqrt(vec_x * vec_x + vec_y * vec_y);

                if (dist > 0.0) {
                    double unit_vec_x = vec_x / dist;
                    double unit_vec_y = vec_y / dist;

                    s->body[i].m_x = (int)std::round(prev_x + unit_vec_x * SEGMENT_SIZE);
                    s->body[i].m_y = (int)std::round(prev_y + unit_vec_y * SEGMENT_SIZE);
                }
            }
        }
        x = next_x;
        y = next_y;
    }
}
void ObjManager::DeleteSnake(unsigned long long id)
{
    m_snakes.erase(m_snakes.begin() + 1);
}

void ObjManager::DeleteFood(unsigned long long id)
{
    m_foods.erase(m_foods.begin() + id);
}

void ObjManager::SnakeEatFood(unsigned long long id)
{
    Snake* sn = {};
    for (Snake s : m_snakes)
        if (s.m_id == id)
            sn = &s;
    auto& body = sn->body;
    size_t size = body.size();

    if (size < 2) {
        body.emplace_back(Object(id, body.front().m_x, body.front().m_y, body.front().m_color));
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


    double new_x = tail.m_x - (unit_dx * SEGMENT_SIZE); // ≤ø∏Æ->æ’≤ø∏Æ ∫§≈Õ∏¶ ª≠
    double new_y = tail.m_y - (unit_dy * SEGMENT_SIZE); // ≤ø∏Æ->æ’≤ø∏Æ ∫§≈Õ∏¶ ª≠

    COLORREF c = body.front().m_color;
    body.emplace_back(Object(id, new_x, new_y, c));
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
    auto& snake_head = m_snakes[0].body.front(); // ¬¸¡∂∏¶ ªÁøÎ«œ∞Ì front() ªÁøÎ
    for (int f = (int)m_foods.size() - 1; f >= 0; f--)
    {
        if (&snake_head) {
            if (snake_head.CheckCollision(m_foods[f]))
            {
                m_foods[f].isalive = false;
                SnakeEatFood(0);
            }
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
                return food.isalive == false; // isalive∞° false¿Œ ø‰º“∏¶ ªË¡¶
            }),
        m_foods.end());
}
