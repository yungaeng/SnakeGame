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
	S2C_ADD_SNAKE_BDOY_PACKET sendPkt;
	sendPkt.id = GetID();
	sendPkt.bodyIndex = m_body.size();
	sendPkt.x = pos.x;
	sendPkt.y = pos.y;
	MANAGER(GameMap)->AppendPkt(sendPkt);
	
	m_body.emplace_back(pos);
}

void Player::Update(const float dt)
{
    Pos targetPos = GetPos();

    const float segmentDist = 20.f;
    const float followSpeed = 4000.f * dt;

    const float maxStep = followSpeed;

    for(int i = 0; i < static_cast<int>(m_body.size()); ++i) {
        Pos& seg = m_body[i];

        Pos dir = targetPos - seg;
        float dist = dir.Length();

        if(dist > 0.0001f) {
            if(dist > segmentDist) {
                dir.Normalize();

                float distToClose = dist - segmentDist; 
                float move = std::min(maxStep, distToClose);

                seg += dir * move;
            }
        }

        S2C_SNAKE_BODY_PACKET sendPkt{};
        sendPkt.id = GetID();
        sendPkt.bodyIndex = i;
        sendPkt.x = static_cast<int>(std::round(seg.x));
        sendPkt.y = static_cast<int>(std::round(seg.y));
        MANAGER(GameMap)->AppendPkt(sendPkt);

        targetPos = seg;
    }
}