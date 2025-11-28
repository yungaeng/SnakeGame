#pragma once
#include "Timer.h"
#include "SendBuffer.h"

class GameObject;
class Player;

class GameMap {
	SINGLETON(GameMap)
public:
	static constexpr uint32 MAP_SIZE{ 600 };
private:
	std::atomic_ullong								G_globalID;
	
	Timer											m_timer;
	static constexpr float							UPDATE_INVERVAL{ 1.f / 60.f }; // 60fps
	float											m_accDTForUpdate{ 0.f };
	
	static constexpr float							FOOD_SPAWN_INTERVAL{ 1.f };
	float											m_accDTForFoodSpawn{ 0.f };

	std::map<uint64, std::shared_ptr<GameObject>>	m_foods;
	std::map<uint64, std::shared_ptr<Player>>		m_players;

	std::shared_mutex								m_nameSetMtx;
	std::unordered_set<std::wstring>				m_playerNames;

	std::mutex										m_sendBuffMtx;
	SendBuffer										m_sendBuffer;

	std::recursive_mutex							m_eveMtx;
	std::queue<std::function<void()>>				m_eventFpQueue;

public:
	void AddEvent(std::function<void()> eve);
	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);
	void Update(const std::stop_token& st);

	template<typename PacketType>
	void AppendPkt(PacketType&& pkt)
	{
		std::lock_guard<std::mutex> lk{ m_sendBuffMtx };
		m_sendBuffer.Append<PacketType>(std::forward<PacketType>(pkt));
	}
 
public:
	uint64 GetGlobalID() noexcept { return ++G_globalID; }

public:
	bool FindPlayerName(std::wstring_view name);

private:
	void ProcessEvent();
	void CheckCollision();
	void SpawnFood();
	void FlushSendBuffer();

};