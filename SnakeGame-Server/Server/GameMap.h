#pragma once
class GameObject;

class GameMap {
	SINGLETON(GameMap)
private:
	std::atomic_ullong		G_globalID;

public:
	void Update(const std::stop_token& st);

public:
	uint64 GetGlobalID() noexcept { return ++G_globalID; }
};