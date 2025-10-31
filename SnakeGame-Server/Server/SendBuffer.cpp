#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer()
	:m_writeSize{0}
{
	m_buffer.resize(BUFFER_SIZE * BUFFER_COUNT);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Append(const char* const buff, const uint32 dataSize)
{
	memcpy(&m_buffer[m_writeSize], buff, dataSize);
	m_writeSize += dataSize;
}