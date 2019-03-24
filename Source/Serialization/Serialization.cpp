#include "Precompiled.hpp"
#include "Serialization/Serialization.hpp"

bool serialize(MemoryBuffer& buffer, Serializable& serializable)
{
    return serializable.onSerialize(buffer);
}

bool deserialize(MemoryBuffer& buffer, Serializable* serializable)
{
    ASSERT(serializable != nullptr);
    return serializable->onDeserialize(buffer);
}

bool serialize(MemoryBuffer& buffer, uint8_t value)
{
    buffer.writeByte(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint8_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte(value);
}

bool serialize(MemoryBuffer& buffer, uint16_t value)
{
    buffer.writeShort(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint16_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readShort(value);
}

bool serialize(MemoryBuffer& buffer, uint32_t value)
{
    buffer.writeInteger(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint32_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger(value);
}

bool serialize(MemoryBuffer& buffer, uint64_t value)
{
    buffer.writeWord(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint64_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord(value);
}

bool serialize(MemoryBuffer& buffer, int8_t value)
{
    buffer.writeByte((uint8_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int8_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte((uint8_t*)value);
}

bool serialize(MemoryBuffer& buffer, int16_t value)
{
    buffer.writeShort((uint16_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int16_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readShort((uint16_t*)value);
}

bool serialize(MemoryBuffer& buffer, int32_t value)
{
    buffer.writeInteger((uint32_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int32_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryBuffer& buffer, int64_t value)
{
    buffer.writeWord((uint64_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int64_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}

bool serialize(MemoryBuffer& buffer, char value)
{
    buffer.writeByte((uint8_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, char* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte((uint8_t*)value);
}

bool serialize(MemoryBuffer& buffer, float value)
{
    buffer.writeInteger((uint32_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, float* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryBuffer& buffer, double value)
{
    buffer.writeWord((uint64_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, double* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}

bool serialize(MemoryBuffer& buffer, const sf::Vector2f& value)
{
    if(!serialize(buffer, value.x))
        return false;

    if(!serialize(buffer, value.y))
        return false;

    return true;
}

bool deserialize(MemoryBuffer& buffer, sf::Vector2f* value)
{
    ASSERT(value != nullptr);

    if(!deserialize(buffer, &value->x))
        return false;

    if(!deserialize(buffer, &value->y))
        return false;

    return true;
}

bool serialize(MemoryBuffer& buffer, const std::string& value)
{
    if(!serialize(buffer, value.length()))
        return false;

    for(int i = 0; i < value.length(); ++i)
    {
        if(!serialize(buffer, value[i]))
            return false;
    }

    return true;
}

bool deserialize(MemoryBuffer& buffer, std::string* value)
{
    ASSERT(value != nullptr);

    std::size_t length;
    if(!deserialize(buffer, &length))
        return false;

    value->resize(length);
    for(int i = 0; i < length; ++i)
    {
        if(!deserialize(buffer, &(*value)[i]))
            return false;
    }

    return true;
}
