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
}

void ObjManager::HandleCollisions()
{
    // 여기선 0만 판단, 0만 움직이으로
    for (size_t j = 1; j < objs.size(); ++j) {

        // 객체 0과 객체 j가 충돌하는지 확인
        if (objs[0].CheckCollision(objs[j])) {
            objs[0].m_r = 255; objs[0].m_g = 0; objs[0].m_b = 0; // 빨간색으로 변경
            objs[j].m_r = 0; objs[j].m_g = 0; objs[j].m_b = 255; // 파란색으로 변경
            objs[j].head = &objs[0];
        }
    }
}

