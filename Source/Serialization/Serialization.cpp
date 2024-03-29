#include "Precompiled.hpp"
#include "Serialization/Serialization.hpp"

bool serialize(MemoryStream& buffer, const Serializable& serializable)
{
    return serializable.onSerialize(buffer);
}

bool deserialize(MemoryStream& buffer, Serializable* serializable)
{
    ASSERT(serializable != nullptr);
    return serializable->onDeserialize(buffer);
}

bool serialize(MemoryStream& buffer, uint8_t value)
{
    return buffer.writeByte(&value);
}

bool deserialize(MemoryStream& buffer, uint8_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte(value);
}

bool serialize(MemoryStream& buffer, uint16_t value)
{
    return buffer.writeShort(&value);
}

bool deserialize(MemoryStream& buffer, uint16_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readShort(value);
}

bool serialize(MemoryStream& buffer, uint32_t value)
{
    return buffer.writeInteger(&value);
}

bool deserialize(MemoryStream& buffer, uint32_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger(value);
}

bool serialize(MemoryStream& buffer, uint64_t value)
{
    return buffer.writeWord(&value);
}

bool deserialize(MemoryStream& buffer, uint64_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord(value);
}

bool serialize(MemoryStream& buffer, int8_t value)
{
    return buffer.writeByte((uint8_t*)&value);
}

bool deserialize(MemoryStream& buffer, int8_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte((uint8_t*)value);
}

bool serialize(MemoryStream& buffer, int16_t value)
{
    return buffer.writeShort((uint16_t*)&value);
}

bool deserialize(MemoryStream& buffer, int16_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readShort((uint16_t*)value);
}

bool serialize(MemoryStream& buffer, int32_t value)
{
    return buffer.writeInteger((uint32_t*)&value);
}

bool deserialize(MemoryStream& buffer, int32_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryStream& buffer, int64_t value)
{
    return buffer.writeWord((uint64_t*)&value);
}

bool deserialize(MemoryStream& buffer, int64_t* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}

bool serialize(MemoryStream& buffer, char value)
{
    return buffer.writeByte((uint8_t*)&value);
}

bool deserialize(MemoryStream& buffer, char* value)
{
    ASSERT(value != nullptr);
    return buffer.readByte((uint8_t*)value);
}

bool serialize(MemoryStream& buffer, float value)
{
    return buffer.writeInteger((uint32_t*)&value);
}

bool deserialize(MemoryStream& buffer, float* value)
{
    ASSERT(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryStream& buffer, double value)
{
    return buffer.writeWord((uint64_t*)&value);
}

bool deserialize(MemoryStream& buffer, double* value)
{
    ASSERT(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}

bool serialize(MemoryStream& buffer, const sf::Vector2f& value)
{
    if(!serialize(buffer, value.x))
        return false;

    if(!serialize(buffer, value.y))
        return false;

    return true;
}

bool deserialize(MemoryStream& buffer, sf::Vector2f* value)
{
    ASSERT(value != nullptr);

    if(!deserialize(buffer, &value->x))
        return false;

    if(!deserialize(buffer, &value->y))
        return false;

    return true;
}

bool serialize(MemoryStream& buffer, const std::string& value)
{
    if(!serialize(buffer, value.length()))
        return false;

    for(char i : value)
    {
        if(!serialize(buffer, i))
            return false;
    }

    return true;
}

bool deserialize(MemoryStream& buffer, std::string* value)
{
    ASSERT(value != nullptr);

    std::size_t length;
    if(!deserialize(buffer, &length))
        return false;

    value->resize(length);
    for(std::size_t i = 0; i < length; ++i)
    {
        if(!deserialize(buffer, &(*value)[i]))
            return false;
    }

    return true;
}
