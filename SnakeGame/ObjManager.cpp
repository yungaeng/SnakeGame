#include "ObjManager.h"
constexpr auto MAP_SIZE = 700;

std::unordered_map<unsigned long long, Object> ObjManager::m_foods;
std::unordered_map<unsigned long long, Snake> ObjManager::m_snakes;

void ObjManager::AddFood(unsigned long long id, float  x, float  y, COLORREF color)
{
    Object o(x, y, color);
    m_foods.try_emplace(id, o);
}
void ObjManager::AddSnake(unsigned long long id, wchar_t* name, float  x, float  y, COLORREF color)
{
    Snake s(name, x, y, color);
    if(m_snakes.contains(id)==false)
        m_snakes.insert(std::make_pair(id, s));
}

void ObjManager::DeleteSnake(unsigned long long id)
{
    m_snakes.erase(id);
}
void ObjManager::DeleteFood(unsigned long long id)
{
    m_foods.erase(id);
}

//void ObjManager::MoveSnake(unsigned long long id, double deltaTime)
//{
//    // 락은 Game::Update에서 잡고 있으므로, 여기서는 id의 유효성만 검사합니다.
//    if (m_snakes.count(id) == 0) { // 맵에 ID가 없는 경우 즉시 반환
//        return;
//    }
//
//    int mx = m_snakes[id].GetTargetX();
//    int my = m_snakes[id].GetTargetY();
//    int x = m_snakes[id].m_body.begin()->GetX();
//    int y = m_snakes[id].m_body.begin()->GetY();
//    double vel = m_snakes[id].m_body.begin()->GetSpeed();
//
//    double dx = (double)mx - x;
//    double dy = (double)my - y;
//    double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
//
//    if (distance < 1.0) {
//        x = mx;
//        y = my;
//        return;
//    }
//
//    double moveDistance = vel * deltaTime;
//    double finalMoveDistance = std::min(moveDistance, distance);
//    double ratio = finalMoveDistance / distance;
//
//    int next_x = x + (int)std::round(dx * ratio);
//    int next_y = y + (int)std::round(dy * ratio);
//
//    if (finalMoveDistance == distance) {
//        m_snakes[id].m_body.begin()->SetPos(next_x, next_y);
//    }
//
//    if (next_y > 10 && next_y < MAP_SIZE - 40 &&
//        next_x > 5 && next_x < MAP_SIZE - 10)
//    {
//        if (m_snakes[id].m_body.begin()->GetSize() > 1) {
//            for (size_t i = m_snakes[id].m_body.size() - 1; i > 0; i--) {
//                const double SEGMENT_SIZE = m_snakes[id].m_body.begin()->GetSize();
//
//                double prev_x = m_snakes[id].m_body.at(i - 1).GetX();
//                double prev_y = m_snakes[id].m_body.at(i - 1).GetY();
//                double current_x = m_snakes[id].m_body.at(i).GetX();
//                double current_y = m_snakes[id].m_body.at(i).GetY();
//
//                double vec_x = current_x - prev_x;
//                double vec_y = current_y - prev_y;
//                double dist = std::sqrt(vec_x * vec_x + vec_y * vec_y);
//
//                if (dist > 0.0) {
//                    double unit_vec_x = vec_x / dist;
//                    double unit_vec_y = vec_y / dist;
//
//                    m_snakes[id].m_body.at(i).SetPos((int)std::round(prev_x + unit_vec_x * SEGMENT_SIZE),
//                        (int)std::round(prev_y + unit_vec_y * SEGMENT_SIZE));
//                }
//            }
//        }
//        m_snakes[id].m_body.at(0).SetPos(next_x, next_y);
//    }
//}
//void ObjManager::FoodCollisions()
//{
//    auto& snake_head = m_snakes[0].body.front(); // 참조를 사용하고 front() 사용
//    for (int f = (int)m_foods.size() - 1; f >= 0; f--)
//    {
//        if (&snake_head) {
//            if (snake_head.CheckCollision(m_foods[f]))
//            {
//                m_foods[f].isalive = false;
//                SnakeEatFood(0);
//            }
//        }
//    }
//}
//int ObjManager::SnakeCollisions()
//{
//    Snake my_snake = m_snakes[0];
//    for (int i = 2; i < my_snake.body.size(); i++)
//    {
//        if (my_snake.body[i].CheckCollision(my_snake.body.front()))
//        {
//            gameover = true;
//            return 0;
//        }
//    }
//
//    for (int other = 1; other < m_snakes.size(); other++) {
//        for (int i = 2; i < my_snake.body.size(); i++)
//        {
//            if (m_snakes[other].body.begin()->CheckCollision(my_snake.body[i]))
//            {
//                gameover = true;
//                return other;
//            }
//        }
//    }
//    return -1;
//}
//void ObjManager::GarbageCollector()
//{
//    m_foods.erase(std::remove_if(m_foods.begin(), m_foods.end(),
//            [](const Object& food) {
//                return food.isalive == false; // isalive가 false인 요소를 삭제
//            }),
//        m_foods.end());
//}


