#include "Object.h"

Object::Object(int x, int y, int size, int r, int g, int b)
{
	m_x = x;
	m_y = y;
	m_size = size;
	m_r = r;
	m_g = g;
	m_b = b;
}

bool Object::CheckCollision(const Object& other) const {
    // ���� ��ü�� ���
    int leftA = m_x - m_size;
    int rightA = m_x + m_size;
    int topA = m_y - m_size;
    int bottomA = m_y + m_size;

    // �ٸ� ��ü�� ���
    int leftB = other.m_x - other.m_size;
    int rightB = other.m_x + other.m_size;
    int topB = other.m_y - other.m_size;
    int bottomB = other.m_y + other.m_size;

    // AABB (Axis-Aligned Bounding Box) �浹 Ȯ��:
    // �� �簢���� ��ġ�� �ʴ� 4���� ��� �� �ϳ��� �ش�Ǹ� �浹���� ���� ���Դϴ�.
    if (bottomA <= topB || // A�� �Ʒ����� B�� ���ʺ��� ���� �ְų� (A�� B���� ��)
        topA >= bottomB || // A�� ������ B�� �Ʒ��ʺ��� �Ʒ��� �ְų� (A�� B���� �Ʒ�)
        rightA <= leftB || // A�� �������� B�� ���ʺ��� ���ʿ� �ְų� (A�� B���� ����)
        leftA >= rightB)   // A�� ������ B�� �����ʺ��� �����ʿ� �ְų� (A�� B���� ������)
    {
        return false; // �浹���� ����
    }

    return true; // �浹��
}

