#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(GAME_OBJECT_TYPE type)
	:m_type{type}, m_alive(true)
{
}

GameObject::~GameObject()
{
	std::cout << "~GameObject" << std::endl;
}

bool GameObject::IsCollision(const Pos otherPos)
{
	const float myLeft{ m_pos.x - GAME_OBJECT_SIZE / 2.f };
	const float myRight{ m_pos.x + GAME_OBJECT_SIZE / 2.f };
	const float myTop{ m_pos.y - GAME_OBJECT_SIZE / 2.f };
	const float myBottom{ m_pos.y + GAME_OBJECT_SIZE / 2.f };

	const float otherLeft{ otherPos.x - GAME_OBJECT_SIZE / 2.f };
	const float otherRight{ otherPos.x + GAME_OBJECT_SIZE / 2.f };
	const float otherTop{ otherPos.y - GAME_OBJECT_SIZE / 2.f };
	const float otherBottom{ otherPos.y + GAME_OBJECT_SIZE / 2.f };

	if(myBottom <= otherTop || myTop >= otherBottom || myRight <= otherLeft || myLeft >= otherRight)
		return false;
	return true;
}
