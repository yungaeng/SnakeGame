#include "pch.h"
#include "Player.h"

Player::Player()
	:GameObject(GAME_OBJECT_TYPE::PLAYER)
{
}

Player::~Player()
{
	std::cout << "~Player" << std::endl;
}
