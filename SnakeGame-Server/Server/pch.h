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
#include <map>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <numeric>
#include <array>
#include <set>
#include <unordered_set>
#include <random>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

constexpr unsigned short SERVER_PORT{ 9000 };


#include "../../protocol/protocol.h"
#include "ClientPacketHandler.h"
#include "SendBuffer.h"

using namespace std::chrono;

struct Vector {
	Vector() {}
	Vector(float x, float y) : x(x), y(y) {}
	Vector(POINT pt) : x((float)pt.x), y((float)pt.y) {}

	Vector operator+(const Vector& other)
	{
		Vector ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	Vector operator-(const Vector& other)
	{
		Vector ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	Vector operator*(float value)
	{
		Vector ret;
		ret.x = x * value;
		ret.y = y * value;
		return ret;
	}

	void operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
	}

	void operator*=(float ratio)
	{
		x *= ratio;
		y *= ratio;
	}

	float LengthSquared()
	{
		return x * x + y * y;
	}

	float Length()
	{
		return ::sqrt(LengthSquared());
	}

	void Normalize()
	{
		float length = Length();
		if(length < 0.00000000001f)
			return;

		x /= length;
		y /= length;
	}

	float Dot(Vector other)
	{
		return x * other.x + y * other.y;
	}

	float Cross(Vector other)
	{
		return x * other.y - y * other.x;
	}

	float x = 0;
	float y = 0;
};

using Pos = Vector;

extern std::default_random_engine dre;