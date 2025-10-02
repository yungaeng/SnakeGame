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
    case 2: objs[index].m_x -= objs[index].m_speed; break;
    case 3: objs[index].m_x += objs[index].m_speed; break;
    default:
        break;
    }

    objs[index].m_dir = dir;

    
    for (size_t j = 0; j < objs.size(); ++j) {
        // 자기 자신은 건너뜁니다.
        if (j == index) {
            continue;
        }

        if (objs[j].m_target == index) {
            int len = objs[j].m_tail_len;
            objs[j].m_x = objs[objs[j].m_target].m_x;
            objs[j].m_y = objs[objs[j].m_target].m_y;
            switch (dir)
            {
            case 0: objs[j].m_y += len; break;
            case 1:  objs[j].m_y -= len; break;
            case 2: objs[j].m_x += len; break;
            case 3: objs[j].m_x -= len; break;
            default:
                break;
            }
            
        }
    }
}

void ObjManager::UpdateObj()
{
    HandleCollisions();
}

void ObjManager::HandleCollisions()
{
    for (size_t i = 0; i < objs.size(); ++i) {
        for (size_t j = i+1; j < objs.size(); ++j) {
            if (objs[i].CheckCollision(objs[j])) {
                // 여기에 충돌 처리 작성
                objs[j].m_target = 0;
                objs[i].m_tail_len += 10;
                objs[j].m_tail_len = objs[i].m_tail_len;
            }
        }
    }
}

