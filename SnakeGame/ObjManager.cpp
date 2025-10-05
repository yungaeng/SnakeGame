#include "ObjManager.h"

std::vector<Object> ObjManager::m_foods;
std::vector<Object> ObjManager::m_snake;

ObjManager::ObjManager()
{
    // 원래는 게임 씬에서 맵 초기화를 해줘야 하지만
    // 여기서 게임 초기화 작업을 정의해 두겠다

    // 1. 음식 생성
    for (int i = 0; i < 20; i++)
    {
        pos p = { rand() % 700,rand() % 700,rand() % 700 };
        color c = { rand() % 255, rand() % 255,rand() % 255 };
        m_foods.emplace_back(Object(p, c));
    }

    // 2. 뱀 생성
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
    // 업데이트 에서 해야 하는 일
    // 1. 오브젝트 데이터 가져오기
    // 
    // 2. 가져온 데이터 기존 데이터와 동기화 처리
    // 
    // 3. 서버와 동기화된 모든 오브젝트 충돌처리
    HandleCollisions();
    // 4. 산출된 결과 서버로 송신처리
    //
    if (m_isgameover)
        *gamestate = true;
}

void ObjManager::HandleCollisions()
{
    // 모든 음식의 충돌체크
    for (size_t i = 0; i < m_foods.size(); ++i) {
        // 1. 뱀 0번과 음식의 충돌체크
        if (m_snake[0].CheckCollision(m_foods[i])) {
            // 여기에 충돌 처리 작성
            m_foods.erase(m_foods.begin() + i);
            SnakeEatFood();
        }
    }

    // 모든 뱀의 몸통 충돌체크
    for (size_t i = 1; i < m_snake.size(); ++i) {
        // 1. 뱀 0번의 머리와 몸통의 충돌체크
        if (m_snake[0].CheckCollision(m_snake[i])) {
            // 여기에 충돌 처리 작성
            // 게임 종료 flag, update에서 마저 종료메시지 전달
            m_isgameover = true;
        }
    }
}