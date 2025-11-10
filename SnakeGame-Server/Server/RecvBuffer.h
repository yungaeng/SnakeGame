#pragma once

class RecvBuffer {
private:
	static constexpr auto	BUFFER_SIZE = 1024 * 64;
	static constexpr auto	BUFFER_COUNT = 10;

	std::vector<char>		m_buffer;
	uint32					m_capacity;
	uint32					m_bufferSize;
	uint32					m_readPos;
	uint32					m_writePos;

public:
	RecvBuffer();
	~RecvBuffer();

public:
	uint32 GetDataSize() const noexcept { return m_writePos - m_readPos; }
	uint32 GetFreeSize() const noexcept { return m_capacity - m_writePos; }
	char* GetReadPos() noexcept { return &m_buffer[m_readPos]; }
	char* GetWritePos() noexcept { return &m_buffer[m_writePos]; }

public:
	bool OnWrite(const uint32 dataSize);
	bool OnRead(const uint32  dataSize);
	void Clean();

};

