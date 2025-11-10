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