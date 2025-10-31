#pragma once

class SendBuffer {
private:
	static constexpr auto	BUFFER_SIZE = 1024;
	static constexpr auto	BUFFER_COUNT = 10;
	std::vector<char>		m_buffer;
	uint32					m_writeSize;

public:
	SendBuffer();
	~SendBuffer();

public:
	template<typename Packet> 
	void Append(Packet&& packet) { Append((char*)&packet, sizeof(Packet)); }

	void Append(const char* const packet, const uint32 packetSize);
private:

public:
	uint32		GetDataSize() { return m_writeSize; }
	const char* GetBuffer() { return m_buffer.data(); }

public:
	void Clear() { m_writeSize = 0; }
};

