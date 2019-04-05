#include "Precompiled.hpp"
#include "Serialization/MemoryBuffer.hpp"

MemoryBuffer::MemoryBuffer() :
    m_index(0)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

bool MemoryBuffer::writeByte(uint8_t* value)
{
    if(m_index != m_buffer.size())
        return false;

    m_buffer.resize(m_index + sizeof(uint8_t));
    *((uint8_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint8_t);

    return true;
}

bool MemoryBuffer::writeShort(uint16_t* value)
{
    if(m_index != m_buffer.size())
        return false;

    m_buffer.resize(m_index + sizeof(uint16_t));
    *((uint16_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint16_t);

    return true;
}

bool MemoryBuffer::writeInteger(uint32_t* value)
{
    if(m_index != m_buffer.size())
        return false;

    m_buffer.resize(m_index + sizeof(uint32_t));
    *((uint32_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint32_t);

    return true;
}

bool MemoryBuffer::writeWord(uint64_t* value)
{
    if(m_index != m_buffer.size())
        return false;

    m_buffer.resize(m_index + sizeof(uint64_t));
    *((uint64_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint64_t);

    return true;
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

void MemoryBuffer::clear()
{
    m_buffer.clear();
    m_index = 0;
}

void MemoryBuffer::reset()
{
    m_index = 0;
}

void MemoryBuffer::resize(std::size_t size)
{
    m_buffer.resize(size, 0);
}

void MemoryBuffer::fill(const char* data, std::size_t size)
{
    m_buffer.clear();
    m_index = 0;

    m_buffer.resize(size);
    memcpy(m_buffer.data(), data, size);
}

std::size_t MemoryBuffer::size() const
{
    return m_buffer.size();
}

const char* MemoryBuffer::data() const
{
    return &m_buffer[0];
}

char* MemoryBuffer::data()
{
    return &m_buffer[0];
}
