#include "Object.h"

Object::Object(int id, int x, int y, int size, int r, int g, int b)
{
    m_id = id;
	m_x = x;
	m_y = y;
	m_size = size;
	m_r = r;
	m_g = g;
	m_b = b;
}

bool Object::CheckCollision(const Object& other) const {
    // 현재 객체의 경계
    int leftA = m_x - m_size;
    int rightA = m_x + m_size;
    int topA = m_y - m_size;
    int bottomA = m_y + m_size;

    // 다른 객체의 경계
    int leftB = other.m_x - other.m_size;
    int rightB = other.m_x + other.m_size;
    int topB = other.m_y - other.m_size;
    int bottomB = other.m_y + other.m_size;

    // AABB (Axis-Aligned Bounding Box) 충돌 확인:
    // 두 사각형이 겹치지 않는 4가지 경우 중 하나라도 해당되면 충돌하지 않은 것입니다.
    if (bottomA <= topB || // A의 아래쪽이 B의 위쪽보다 위에 있거나 (A가 B보다 위)
        topA >= bottomB || // A의 위쪽이 B의 아래쪽보다 아래에 있거나 (A가 B보다 아래)
        rightA <= leftB || // A의 오른쪽이 B의 왼쪽보다 왼쪽에 있거나 (A가 B보다 왼쪽)
        leftA >= rightB)   // A의 왼쪽이 B의 오른쪽보다 오른쪽에 있거나 (A가 B보다 오른쪽)
    {
        return false; // 충돌하지 않음
    }

    return true; // 충돌함
}

