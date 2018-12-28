#include "Buffer.h"

TBuffer::TBuffer(void *data, int tsize, int length, BufferOptions options)
{
	glCreateBuffers(1, &m_id); // Create CPU handle
	if (length == 0)
		return;
	glNamedBufferStorage(m_id, tsize * length, data, options.GetEnum()); // Initialize buffer store
	m_length = length;
	m_tsize = tsize;
}

void TBuffer::CopyData(void *data, int offset, int length) 
{
	size_t size = length * m_tsize;
	glNamedBufferSubData(m_id, offset * m_tsize, size, data);
}

void TBuffer::CopyBufferData(int offset, int size, TBuffer dest, int destOffset)
{
	glCopyNamedBufferSubData(m_id, dest.m_id, offset * m_tsize, destOffset * m_tsize, size * m_tsize);
}

void TBuffer::FlushBuffer(int offset, int length)
{
	glFlushMappedNamedBufferRange(m_id, offset * m_tsize, length * m_tsize);
}

void TBuffer::Destroy()
{
	glDeleteBuffers(1, &m_id);
}

void TBuffer::MapBuffer(int offset, int length, MappingOptions options)
{
	m_mappingPtr = glMapNamedBufferRange(m_id, offset * m_tsize, length * m_tsize, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
}

void TBuffer::UnmapBuffer()
{
	glUnmapNamedBuffer(m_id);
}
