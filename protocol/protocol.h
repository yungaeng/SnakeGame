#pragma once

#pragma pack(push, 1)

struct PacketHeader {
	uint8 packetSize;
	uint8 packetID;
};

enum class PACKET_ID : uint8 {
	//client
	C2S_LOGIN, C2S_MOVE, C2S_RESTART, C2S_LEAVE,
	//server
	S2C_LOGIN_OK, S2C_LOGIN_FAIL, S2C_ENTER, S2C_FOOD, S2C_MOVE
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
struct S2C_ENTER_PACKET : public PacketHeader {
	wchar_t name[10];
	COLORREF color;
	int x, y;
	S2C_ENTER_PACKET() : PacketHeader{ sizeof(S2C_ENTER_PACKET), static_cast<uint8>(PACKET_ID::S2C_ENTER) } {}
};
struct S2C_FOOD_PACKET : public PacketHeader {
	COLORREF color;
	int x, y;
	S2C_FOOD_PACKET() : PacketHeader{ sizeof(S2C_FOOD_PACKET), static_cast<uint8>(PACKET_ID::S2C_FOOD) } {}
};
struct S2C_MOVE_PACKET : public PacketHeader {
	double deltaTime;
	unsigned long long id;
	int x, y;
	S2C_MOVE_PACKET() : PacketHeader{ sizeof(S2C_MOVE_PACKET), static_cast<uint8>(PACKET_ID::S2C_MOVE) } {}
};

#pragma pack(pop)