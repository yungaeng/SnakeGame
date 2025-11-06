#pragma once

#pragma pack(push, 1)

struct PacketHeader {
	uint8 packetSize;
	uint8 packetID;
};

enum class PACKET_ID : uint8 {
	SC_TEST,
	//client
	CS_LOGIN, CS_MOVE, CS_RESTART, CS_LEAVE,
	//server
	SC_ENTER
};

struct SC_TEST : public PacketHeader {
	// PacketHeader header
	int a;
	SC_TEST() : PacketHeader{ sizeof(SC_TEST), static_cast<uint8>(PACKET_ID::SC_TEST) } {}
};

//client
struct CS_LOGIN_PACKET : public PacketHeader {
	// PacketHeader header
	wchar_t name[10];
	COLORREF color;
	CS_LOGIN_PACKET() : PacketHeader{ sizeof(CS_LOGIN_PACKET), static_cast<uint8>(PACKET_ID::CS_LOGIN) } {}
};

//server
struct SC_ENTER_PACKET : public PacketHeader {
	// PacketHeader header
	wchar_t name[10];
	COLORREF color;
	int x, y;
	SC_ENTER_PACKET() : PacketHeader{ sizeof(SC_ENTER_PACKET), static_cast<uint8>(PACKET_ID::SC_ENTER) } {}
};

#pragma pack(pop)