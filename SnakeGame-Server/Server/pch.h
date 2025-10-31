#pragma once

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

#pragma comment(lib, "ws2_32.lib")

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

constexpr unsigned short SERVER_PORT{ 9000 };

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#include "../../protocol/protocol.h"
