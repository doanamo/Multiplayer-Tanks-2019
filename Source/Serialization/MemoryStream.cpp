#include "Precompiled.hpp"
#include "Serialization/MemoryStream.hpp"

MemoryStream::MemoryStream() :
    m_index(0)
{
}

MemoryStream::~MemoryStream()
{
}

bool MemoryStream::writeByte(uint8_t* value)
{
    if(m_index == m_buffer.size() - 1)
        return false;

    m_buffer.resize(m_buffer.size() + sizeof(uint8_t));
    *((uint8_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint8_t);

    return true;
}

bool MemoryStream::writeShort(uint16_t* value)
{
    if(m_index == m_buffer.size() - 1)
        return false;

    m_buffer.resize(m_buffer.size() + sizeof(uint16_t));
    *((uint16_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint16_t);

    return true;
}

bool MemoryStream::writeInteger(uint32_t* value)
{
    if(m_index == m_buffer.size() - 1)
        return false;

    m_buffer.resize(m_buffer.size() + sizeof(uint32_t));
    *((uint32_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint32_t);

    return true;
}

bool MemoryStream::writeWord(uint64_t* value)
{
    if(m_index == m_buffer.size() - 1)
        return false;

    m_buffer.resize(m_buffer.size() + sizeof(uint64_t));
    *((uint64_t*)&m_buffer[m_index]) = *value;
    m_index += sizeof(uint64_t);

    return true;
}

bool MemoryStream::writeData(const char* data, std::size_t size)
{
    if(m_index == m_buffer.size() - 1)
        return false;

    m_buffer.resize(m_buffer.size() + size);
    memcpy(m_buffer.data() + m_index, data, size);
    m_index += size;

    return true;
}

bool MemoryStream::readByte(uint8_t* value)
{
    if(m_index + sizeof(uint8_t) > m_buffer.size())
        return false;

    *value = *((uint8_t*)&m_buffer[m_index]);
    m_index += sizeof(uint8_t);

    return value;
}

bool MemoryStream::readShort(uint16_t* value)
{
    if(m_index + sizeof(uint16_t) > m_buffer.size())
        return false;

    *value = *((uint16_t*)&m_buffer[m_index]);
    m_index += sizeof(uint16_t);

    return true;
}

bool MemoryStream::readInteger(uint32_t* value)
{
    if(m_index + sizeof(uint32_t) > m_buffer.size())
        return false;

    *value = *((uint32_t*)&m_buffer[m_index]);
    m_index += sizeof(uint32_t);

    return true;
}

bool MemoryStream::readWord(uint64_t* value)
{
    if(m_index + sizeof(uint64_t) > m_buffer.size())
        return false;

    *value = *((uint64_t*)&m_buffer[m_index]);
    m_index += sizeof(uint64_t);

    return true;
}

bool MemoryStream::readData(char* data, std::size_t size)
{
    if(m_index + size > m_buffer.size())
        return false;

    memcpy(data, &m_buffer[m_index], size);
    m_index += size;

    return true;
}

void MemoryStream::clear()
{
    m_buffer.clear();
    m_index = 0;
}

void MemoryStream::reset()
{
    m_index = 0;
}

void MemoryStream::resize(std::size_t size)
{
    m_buffer.resize(size, 0);
}

void MemoryStream::replace(const char* data, std::size_t size)
{
    m_buffer.clear();
    m_index = 0;

    m_buffer.resize(size);
    memcpy(m_buffer.data(), data, size);
}

bool MemoryStream::onSerialize(MemoryStream& buffer) const
{
    if(!serialize(buffer, m_buffer.size()))
        return false;

    if(!buffer.writeData(m_buffer.data(), m_buffer.size()))
        return false;

    return true;
}

bool MemoryStream::onDeserialize(MemoryStream& buffer)
{
    std::size_t bufferSize = 0;

    if(!deserialize(buffer, &bufferSize))
        return false;

    m_buffer.resize(bufferSize);
    if(!buffer.readData(m_buffer.data(), m_buffer.size()))
        return false;

    return true;
}

std::size_t MemoryStream::size() const
{
    return m_buffer.size();
}

const char* MemoryStream::data() const
{
    return &m_buffer[0];
}

char* MemoryStream::data()
{
    return &m_buffer[0];
}
