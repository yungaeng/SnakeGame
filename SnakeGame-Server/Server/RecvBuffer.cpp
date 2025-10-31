#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer()
	:m_bufferSize{ BUFFER_SIZE }, m_writePos{}, m_readPos{}
{
	m_capacity = m_bufferSize * BUFFER_COUNT;
	m_buffer.resize(m_capacity);
}

RecvBuffer::~RecvBuffer()
{
	m_buffer.clear();
}

bool RecvBuffer::OnWrite(const uint32 dataSize)
{
	if(dataSize >= GetFreeSize()) return false;
	
	m_writePos += dataSize;

	return true;
}

bool RecvBuffer::OnRead(const uint32 dataSize)
{
	if(dataSize >= GetDataSize()) return false;
	
	m_readPos += dataSize;

	return true;
}

void RecvBuffer::Clean()
{
	const uint32 dataSize = GetDataSize();
	if(dataSize == 0) {
		m_readPos = m_writePos = 0;
	}
	else {
		if(GetFreeSize() < m_bufferSize) {
			::memcpy(&m_buffer[0], &m_buffer[m_readPos], dataSize);
			m_readPos = 0;
			m_writePos = dataSize;
		}
	}
}
