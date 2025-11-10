#pragma once
#define NOMINMAX
#define MANAGER(classname) classname::GetInstance()
#define SINGLETON(classname)				\
private:									\
	classname()=default;					\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname inst;				\
		return &inst;						\
	}										


#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <thread>
#include <format>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <numeric>
#include <array>
#include <set>
#include <unordered_set>
#include <random>

#pragma comment(lib, "ws2_32.lib")


constexpr unsigned short SERVER_PORT{ 9000 };


#include "../../protocol/protocol.h"
#include "ClientPacketHandler.h"
#include "SendBuffer.h"

using namespace std::chrono;

struct Pos {
	int x, y;
};

extern std::default_random_engine dre;