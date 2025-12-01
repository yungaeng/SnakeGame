#pragma once
#include "GameObject.h"

class Session;

class Player : public GameObject {
private:
	std::shared_ptr<Session>						m_session;
	std::vector<Pos>								m_body;
	float											m_moveSpeed;

public:
	Player();
	virtual ~Player();

public:
	void SetSession(std::shared_ptr<Session> session) { m_session = session; }
	std::shared_ptr<Session> GetSession() { return m_session; }
	
	void AddBody(const Pos pos);
	const auto& GetBody() const noexcept { return m_body; }

	void Update(const float dt);
};


