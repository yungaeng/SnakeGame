#include "ObjManager.h"
std::vector<Object> ObjManager::objs;

void ObjManager::AddObj(int x, int y, int size, int r, int g, int b)
{
	Object o(x, y, size, r, g, b);
	objs.emplace_back(o);
}

void ObjManager::AddObjRandom()
{
	Object o(rand() % 700, rand() % 700, 10, rand() % 255, rand() % 255, rand() % 255);
	objs.emplace_back(o);
}

void ObjManager::MoveObj(int index, char dir) {

    int x = objs[index].m_y, y = objs[index].m_y;

    switch (dir)
    {
    case 0: objs[index].m_y -= objs[index].m_speed; break;
    case 1:  objs[index].m_y += objs[index].m_speed; break;
    case 2: objs[index].m_y -= objs[index].m_speed; break;
    case 3: objs[index].m_x += objs[index].m_speed; break;
    default:
        break;
    }

    objs[index].m_dir = dir;
    if (objs[index].tail != nullptr)
    {
        // objs[index] 부터 시작하여 tail 포인터를 따라 다음 객체로 이동
        for (Object* current = objs[index].tail; current != nullptr; current = current->tail)
        {
            current->m_x = x;
            current->m_y = y;
            current->m_dir = dir;
        }
    }
}

void ObjManager::HandleCollisions()
{
    for (size_t i = 0; i < objs.size(); ++i) {
        for (size_t j = 1; j < objs.size(); ++j) {
            if (objs[i].CheckCollision(objs[j])) {
                //충돌한 놈의 방향에 따라 결정
                switch (objs[i].m_dir)
                {
                case 0: objs[j].m_y = objs[i].m_x + 10; break;
                case 1: objs[j].m_y = objs[i].m_x - 10;  break;
                case 2: objs[j].m_x = objs[i].m_x + 10; break;
                case 3: objs[j].m_x = objs[i].m_x - 10; break;
                default:
                    break;
                }
            }
        }
    }
}

