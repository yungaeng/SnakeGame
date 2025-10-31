#pragma once

#pragma pack(push, 1)

struct PacketHeader {
	uint8 packetSize;
	uint8 packetID;
};

enum class PACKET_ID : uint8 {
	SC_TEST,

};

struct SC_TEST : public PacketHeader {
	// PacketHeader header
	int a;
	SC_TEST() : PacketHeader{ sizeof(SC_TEST), static_cast<uint8>(PACKET_ID::SC_TEST) } {}
};

#pragma pack(pop)