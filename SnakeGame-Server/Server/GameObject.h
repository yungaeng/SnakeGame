#pragma once

class Session;

class GameObject {
private:
	std::wstring			m_name;
	uint64					m_id;
	GAME_OBJECT_TYPE		m_type;
	bool					m_alive;
	COLORREF				m_color;
	Pos						m_pos;

public:
	explicit GameObject(GAME_OBJECT_TYPE type);
	virtual ~GameObject();

public:
	void SetName(std::wstring_view name) { m_name = name; }
	void SetColor(const COLORREF color) { m_color = color; }
	void SetAlive(const bool alive) noexcept { m_alive = alive; }
	bool IsAlive() const noexcept { return m_alive; }
	void SetID(const uint64 id) noexcept { m_id = id; }
	uint64 GetID() const noexcept { return m_id; }
	GAME_OBJECT_TYPE GetType() const noexcept { return m_type; }
	COLORREF GetColor() const noexcept { return m_color; }
	const std::wstring& GetName() const noexcept { return m_name; }
	void SetPos(const Pos pos) { m_pos = pos; }
	Pos GetPos() const noexcept { return m_pos; }

public:
	bool IsCollision(const std::shared_ptr<GameObject>& other);
};