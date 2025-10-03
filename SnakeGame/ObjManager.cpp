#include "ObjManager.h"
#include <atomic>

std::unordered_map<int, Object> ObjManager::objs;

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
    /*for (size_t i = 0; i < objs.size(); ++i) {
        objs[i].m_prev_x = objs[i].m_x;
        objs[i].m_prev_y = objs[i].m_y;
    }*/

    HandleCollisions();
}

std::atomic<int> tail;

void ObjManager::HandleCollisions()
{
    for (size_t i = 0; i < objs.size(); ++i) {
        for (size_t j = i+1; j < objs.size(); ++j) {
            if (objs[i].CheckCollision(objs[j])) {
                // 여기에 충돌 처리 작성
                
                objs[j].m_target = tail;
                tail = objs[j].m_id;

                objs[j].m_x = objs[i].m_prev_x;
                objs[j].m_y = objs[i].m_prev_y;
            }
        }
    }
}

