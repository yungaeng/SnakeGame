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

void Player::AddBody(const Pos pos)
{
    { m_body.emplace_back(pos); }

    S2C_SNAKE_BODY_PACKET sendPkt;
	sendPkt.id = GetID();
	sendPkt.bodyindex = m_body.size() - 1;
	sendPkt.x = pos.x;
    sendPkt.y = pos.y;
	MANAGER(GameMap)->AppendPkt(sendPkt);

}

void Player::Update()
{
    Pos headPos = GetPos();
    Pos target = headPos;

    static constexpr float followSpeed = 0.016f;

    for (auto& bodyPos : m_body.size(); ++i) {
        float dx = static_cast<float>(target.x - m_body[i].x);
        float dy = static_cast<float>(target.y - m_body[i].y);

        const float lenSq = dx * dx + dy * dy;

        if(lenSq > 0.0001f * 0.0001f) {
            const float len = std::sqrt(lenSq);

            float nx = dx / len;
            float ny = dy / len;

            m_body[i].x += static_cast<int>(nx * followSpeed);
            m_body[i].y += static_cast<int>(ny * followSpeed);
        }
        target = m_body[i];

		//TODO : Send Packet
		S2C_SNAKE_BODY_PACKET sendPkt;
		sendPkt.id = GetID();
		sendPkt.bodyindex = i;
        sendPkt.x = target.x;
		sendPkt.y = target.y;
		MANAGER(GameMap)->AppendPkt(sendPkt);
    }
}