#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(GAME_OBJECT_TYPE type)
	:m_type{type}
{
}

GameObject::~GameObject()
{
	std::cout << "~GameObject" << std::endl;
}

bool GameObject::IsCollision(const Pos otherPos)
{
	const int32 myLeft{ m_pos.x - GAME_OBJECT_SIZE / 2 };
	const int32 myRight{ m_pos.x + GAME_OBJECT_SIZE / 2 };
	const int32 myTop{ m_pos.y - GAME_OBJECT_SIZE / 2 };
	const int32 myBottom{ m_pos.y + GAME_OBJECT_SIZE / 2 };

	const int32 otherLeft{ otherPos.x - GAME_OBJECT_SIZE / 2 };
	const int32 otherRight{ otherPos.x + GAME_OBJECT_SIZE / 2 };
	const int32 otherTop{ otherPos.y - GAME_OBJECT_SIZE / 2 };
	const int32 otherBottom{ otherPos.y + GAME_OBJECT_SIZE / 2 };

	if(myBottom <= otherTop || myTop >= otherBottom || myRight <= otherLeft || myLeft >= otherRight)
		return false;
	return true;
}
