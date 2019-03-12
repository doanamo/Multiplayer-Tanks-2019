#include "Precompiled.hpp"
#include "Serialization/MemoryBuffer.hpp"

MemoryBuffer::MemoryBuffer() :
    m_index(0)
{
}

MemoryBuffer::~MemoryBuffer()
{
}

void MemoryBuffer::writeByte(uint8_t value)
{
    m_buffer.resize(m_index + sizeof(uint8_t));
    *((uint8_t*)&m_buffer[m_index]) = value;
    m_index += sizeof(uint8_t);
}

void MemoryBuffer::writeShort(uint16_t value)
{
    m_buffer.resize(m_index + sizeof(uint16_t));
    *((uint16_t*)&m_buffer[m_index]) = value;
    m_index += sizeof(uint16_t);
}

void MemoryBuffer::writeInteger(uint32_t value)
{
    m_buffer.resize(m_index + sizeof(uint32_t));
    *((uint32_t*)&m_buffer[m_index]) = value;
    m_index += sizeof(uint32_t);
}

void MemoryBuffer::writeWord(uint64_t value)
{
    m_buffer.resize(m_index + sizeof(uint64_t));
    *((uint64_t*)&m_buffer[m_index]) = value;
    m_index += sizeof(uint64_t);
}

uint8_t MemoryBuffer::readByte()
{
    assert(m_index + sizeof(uint8_t) <= m_buffer.size());
    uint8_t value = *((uint8_t*)&m_buffer[m_index]);
    m_index += sizeof(uint8_t);
    return value;
}

uint16_t MemoryBuffer::readShort()
{
    assert(m_index + sizeof(uint16_t) <= m_buffer.size());
    uint16_t value = *((uint16_t*)&m_buffer[m_index]);
    m_index += sizeof(uint16_t);
    return value;
}

uint32_t MemoryBuffer::readInteger()
{
    assert(m_index + sizeof(uint32_t) <= m_buffer.size());
    uint32_t value = *((uint32_t*)&m_buffer[m_index]);
    m_index += sizeof(uint32_t);
    return value;
}

uint64_t MemoryBuffer::readWord()
{
    assert(m_index + sizeof(uint64_t) <= m_buffer.size());
    uint64_t value = *((uint64_t*)&m_buffer[m_index]);
    m_index += sizeof(uint64_t);
    return value;
}
