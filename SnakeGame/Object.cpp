#include "Object.h"

Object::Object(pos p, COLORREF c)
{
    m_pos = p;
    m_color = c;
}

bool Object::CheckCollision(const Object& other) const
{
    // ���� ��ü�� ���
    int leftA = m_pos.x - m_size;
    int rightA = m_pos.x + m_size;
    int topA = m_pos.y - m_size;
    int bottomA = m_pos.y + m_size;

    // �ٸ� ��ü�� ���
    int leftB = other.m_pos.x - other.m_size;
    int rightB = other.m_pos.x + other.m_size;
    int topB = other.m_pos.y - other.m_size;
    int bottomB = other.m_pos.y + other.m_size;

    // AABB (Axis-Aligned Bounding Box) �浹 Ȯ��:
    if (bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB)
        return false;
    return true;
}

