#pragma once

#include "Precompiled.hpp"
#include "Serialization/MemoryBuffer.hpp"
#include "Serialization/Serializable.hpp"

template<typename Type>
bool serialize(MemoryBuffer& buffer, Type data)
{
    static_assert(false, "Serialization function is not defined for this type!");
}

template<typename Type>
bool deserialize(MemoryBuffer& buffer, Type data)
{
    static_assert(false, "Deserialization function is not defined for this type!");
}

template<>
bool serialize(MemoryBuffer& buffer, uint8_t data)
{
    buffer.writeByte(data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, uint8_t* data)
{
    assert(data != nullptr);
    buffer.readByte(data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, uint16_t data)
{
    buffer.writeShort(data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, uint16_t* data)
{
    assert(data != nullptr);
    buffer.readShort(data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, uint32_t data)
{
    buffer.writeInteger(data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, uint32_t* data)
{
    assert(data != nullptr);
    buffer.readInteger(data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, uint64_t data)
{
    buffer.writeWord(data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, uint64_t* data)
{
    assert(data != nullptr);
    buffer.readWord(data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, int8_t data)
{
    buffer.writeByte((uint8_t)data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, int8_t* data)
{
    assert(data != nullptr);
    buffer.readByte((uint8_t*)data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, int16_t data)
{
    buffer.writeShort((uint16_t)data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, int16_t* data)
{
    assert(data != nullptr);
    buffer.readShort((uint16_t*)data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, int32_t data)
{
    buffer.writeInteger((uint32_t)data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, int32_t* data)
{
    assert(data != nullptr);
    buffer.readInteger((uint32_t*)data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, int64_t data)
{
    buffer.writeWord((uint64_t)data);
    return true;
}

template<>
bool deserialize(MemoryBuffer& buffer, int64_t* data)
{
    assert(data != nullptr);
    buffer.readWord((uint64_t*)data);
    return true;
}

template<>
bool serialize(MemoryBuffer& buffer, Serializable& data)
{
    return data.onSerialize(buffer);
}

template<>
bool deserialize(MemoryBuffer& buffer, Serializable* data)
{
    assert(data != nullptr);
    return data->onDeserialize(buffer);
}
