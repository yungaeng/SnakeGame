#pragma once
#include "GameObject.h"

class Session;
class Player : public GameObject {
private:
	std::shared_ptr<Session>						m_session;
	bool											m_alive;	
	std::vector<Pos>								m_body;

public:
	Player();
	virtual ~Player();

public:
	void SetAlive(const bool alive) { m_alive = alive; }
	bool IsAlive() const noexcept { return m_alive; }
	void SetSession(std::shared_ptr<Session> session) { m_session = session; }
	std::shared_ptr<Session> GetSession() { return m_session; }
	const auto& GetBody() const noexcept { return m_body; }
	void AddBody(const Pos pos);

	void Update();
};


