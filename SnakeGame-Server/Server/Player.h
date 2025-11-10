#pragma once
#include "GameObject.h"

class Session;
class Player : public GameObject {
private:
	std::weak_ptr<Session>	m_session;

public:
	Player();
	virtual ~Player();

public:
	void SetSession(std::shared_ptr<Session> session) { m_session = session; }
	std::shared_ptr<Session> GetSession() { return m_session.lock(); }
};

