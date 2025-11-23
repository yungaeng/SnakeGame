#include "pch.h"
#include "Player.h"

#include "GameMap.h"
Player::Player()
	:GameObject(GAME_OBJECT_TYPE::PLAYER)
{
}

Player::~Player()
{
	std::cout << "~Player" << std::endl;
}

void Player::Update()
{
    Pos headPos = GetPos();
    Pos target = headPos;

    static constexpr float followSpeed = 0.1f;

    for(auto& bodyPos : m_body) {
        float dx = static_cast<float>(target.x - bodyPos.x);
        float dy = static_cast<float>(target.y - bodyPos.y);

        const float lenSq = dx * dx + dy * dy;

        if(lenSq > 0.0001f * 0.0001f) {
            const float len = std::sqrt(lenSq);

            float nx = dx / len;
            float ny = dy / len;

            bodyPos.x += static_cast<int>(nx * followSpeed);
            bodyPos.y += static_cast<int>(ny * followSpeed);
        }
        target = bodyPos;
    }
}