#include "Precompiled.hpp"
#include "Serialization/MemoryBuffer.hpp"

MemoryBuffer::MemoryBuffer() :
    m_index(0)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

void MemoryBuffer::writeByte(uint8_t* value)
{
    m_buffer.resize(m_index + sizeof(uint8_t));
    *((uint8_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint8_t);
}

void MemoryBuffer::writeShort(uint16_t* value)
{
    m_buffer.resize(m_index + sizeof(uint16_t));
    *((uint16_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint16_t);
}

void MemoryBuffer::writeInteger(uint32_t* value)
{
    m_buffer.resize(m_index + sizeof(uint32_t));
    *((uint32_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint32_t);
}

void MemoryBuffer::writeWord(uint64_t* value)
{
    m_buffer.resize(m_index + sizeof(uint64_t));
    *((uint64_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint64_t);
}

bool MemoryBuffer::readByte(uint8_t* value)
{
    if(m_index + sizeof(uint8_t) > m_buffer.size())
        return false;

    *value = *((uint8_t*)&m_buffer[m_index]);
    m_index += sizeof(uint8_t);

    return value;
}

bool MemoryBuffer::readShort(uint16_t* value)
{
    if(m_index + sizeof(uint16_t) > m_buffer.size())
        return false;

    *value = *((uint16_t*)&m_buffer[m_index]);
    m_index += sizeof(uint16_t);

    return true;
}

bool MemoryBuffer::readInteger(uint32_t* value)
{
    if(m_index + sizeof(uint32_t) > m_buffer.size())
        return false;

    *value = *((uint32_t*)&m_buffer[m_index]);
    m_index += sizeof(uint32_t);

    return true;
}

bool MemoryBuffer::readWord(uint64_t* value)
{
    if(m_index + sizeof(uint64_t) > m_buffer.size())
        return false;

    *value = *((uint64_t*)&m_buffer[m_index]);
    m_index += sizeof(uint64_t);

    return true;
}

void MemoryBuffer::resize(std::size_t size)
{
    ASSERT(m_index == 0, "Resizing buffer that is being written or read from!");
    m_buffer.resize(size);
}

std::size_t MemoryBuffer::getSize() const
{
    return m_buffer.size();
}

const char* MemoryBuffer::getData() const
{
    return &m_buffer[0];
}

char* MemoryBuffer::getData()
{
    return &m_buffer[0];
}
