#pragma once
#include "Timer.h"
#include "SendBuffer.h"

class GameObject;
class Player;

class GameMap {
	SINGLETON(GameMap)
private:
	static constexpr float						UPDATE_INVERVAL{ 0.016f }; // 60fps
	static constexpr float						FOOD_SPAWN_INTERVAL{ 1.f };

	float										m_accDTForUpdate{ 0.f };
	float										m_accDTForFoodSpawn{ 0.f };
	Timer										m_timer;
	std::atomic_ullong							G_globalID;
	
	std::mutex									m_vecMutex;
	std::vector<std::shared_ptr<GameObject>>	m_gameObjects;

	std::mutex									m_setMutex;
	std::unordered_set<std::wstring>			m_playerNames;

	std::mutex									m_sendBufferMutex;
	SendBuffer									m_sendBuffer;

public:
	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void Update(const std::stop_token& st);

	template<typename PacketType>
	void AppendPkt(PacketType&& pkt)
	{
		std::lock_guard<std::mutex> lk{ m_sendBufferMutex };
		m_sendBuffer.Append<PacketType>(std::forward<PacketType>(pkt));
	}

public:
	uint64 GetGlobalID() noexcept { return ++G_globalID; }

public:
	bool FindName(std::wstring_view name);

private:
	void ProcessEvent();
	void CheckCollision();
	void SpawnFood();
	void FlushSendBuffer();

};