#pragma once

enum class GAME_OBJECT_TYPE {
	PLAYER = 1,
	FOOD = 2,

	END
};

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
		
#pragma pack(push, 1)
struct PacketHeader {
	uint8 packetSize;
	uint8 packetID;
};

enum class PACKET_ID : uint8 {
	//client
	C2S_LOGIN, C2S_MOVE, C2S_RESTART, C2S_LEAVE,
	//server
	S2C_LOGIN_OK, S2C_LOGIN_FAIL, S2C_PLAYER, S2C_FOOD, S2C_MOVE, S2C_DEL_SNAKE, S2C_DEL_FOOD
};

//client
struct C2S_LOGIN_PACKET : public PacketHeader {
	wchar_t name[10];
	COLORREF color;
	C2S_LOGIN_PACKET() : PacketHeader{ sizeof(C2S_LOGIN_PACKET), static_cast<uint8>(PACKET_ID::C2S_LOGIN) } {}
};
struct C2S_MOVE_PACKET : public PacketHeader {
	int x, y;
	C2S_MOVE_PACKET() : PacketHeader{ sizeof(C2S_MOVE_PACKET), static_cast<uint8>(PACKET_ID::C2S_MOVE) } {}
};
struct C2S_RESTART_PACKET : public PacketHeader {
	C2S_RESTART_PACKET() : PacketHeader{ sizeof(C2S_RESTART_PACKET), static_cast<uint8>(PACKET_ID::C2S_RESTART) } {}
};
struct C2S_LEAVE_PACKET : public PacketHeader {
	C2S_LEAVE_PACKET() : PacketHeader{ sizeof(C2S_LEAVE_PACKET), static_cast<uint8>(PACKET_ID::C2S_LEAVE) } {}
};


//server
struct S2C_LOGIN_OK_PACKET : public PacketHeader {
	int x, y;
	S2C_LOGIN_OK_PACKET() : PacketHeader{ sizeof(S2C_LOGIN_OK_PACKET), static_cast<uint8>(PACKET_ID::S2C_LOGIN_OK) } {}
};
struct S2C_LOGIN_FAIL_PACKET : public PacketHeader {
	S2C_LOGIN_FAIL_PACKET() : PacketHeader{ sizeof(S2C_LOGIN_FAIL_PACKET), static_cast<uint8>(PACKET_ID::S2C_LOGIN_FAIL) } {}
};
struct S2C_PLAYER_PACKET : public PacketHeader {
	wchar_t name[10];
	COLORREF color;
	int x, y;
	unsigned long long id;
	S2C_PLAYER_PACKET() : PacketHeader{ sizeof(S2C_PLAYER_PACKET), static_cast<uint8>(PACKET_ID::S2C_PLAYER) } {}
};
struct S2C_FOOD_PACKET : public PacketHeader {
	COLORREF color;
	int x, y;
	unsigned long long id;
	S2C_FOOD_PACKET() : PacketHeader{ sizeof(S2C_FOOD_PACKET), static_cast<uint8>(PACKET_ID::S2C_FOOD) } {}
};
struct S2C_MOVE_PACKET : public PacketHeader {
	double deltaTime;
	unsigned long long id;
	int x, y;
	S2C_MOVE_PACKET() : PacketHeader{ sizeof(S2C_MOVE_PACKET), static_cast<uint8>(PACKET_ID::S2C_MOVE) } {}
};
struct S2C_DEL_FOOD_PACKET : public PacketHeader {
	unsigned long long id;
	S2C_DEL_FOOD_PACKET() : PacketHeader{ sizeof(S2C_DEL_FOOD_PACKET), static_cast<uint8>(PACKET_ID::S2C_DEL_FOOD) } {}
};
struct S2C_DEL_SNAKE_PACKET : public PacketHeader {
	unsigned long long id;
	S2C_DEL_SNAKE_PACKET() : PacketHeader{ sizeof(S2C_DEL_SNAKE_PACKET), static_cast<uint8>(PACKET_ID::S2C_DEL_SNAKE) } {}
};

#pragma pack(pop)