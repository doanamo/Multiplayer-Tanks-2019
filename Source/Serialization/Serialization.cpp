#include "Precompiled.hpp"
#include "Serialization/Serialization.hpp"

bool serialize(MemoryBuffer& buffer, Serializable& serializable)
{
    return serializable.onSerialize(buffer);
}

bool deserialize(MemoryBuffer& buffer, Serializable* serializable)
{
    assert(serializable != nullptr);
    return serializable->onDeserialize(buffer);
}

bool serialize(MemoryBuffer& buffer, uint8_t value)
{
    buffer.writeByte(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint8_t* value)
{
    assert(value != nullptr);
    return buffer.readByte(value);
}

bool serialize(MemoryBuffer& buffer, uint16_t value)
{
    buffer.writeShort(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint16_t* value)
{
    assert(value != nullptr);
    return buffer.readShort(value);
}

bool serialize(MemoryBuffer& buffer, uint32_t value)
{
    buffer.writeInteger(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint32_t* value)
{
    assert(value != nullptr);
    return buffer.readInteger(value);
}

bool serialize(MemoryBuffer& buffer, uint64_t value)
{
    buffer.writeWord(&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, uint64_t* value)
{
    assert(value != nullptr);
    return buffer.readWord(value);
}

bool serialize(MemoryBuffer& buffer, int8_t value)
{
    buffer.writeByte((uint8_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int8_t* value)
{
    assert(value != nullptr);
    return buffer.readByte((uint8_t*)value);
}

bool serialize(MemoryBuffer& buffer, int16_t value)
{
    buffer.writeShort((uint16_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int16_t* value)
{
    assert(value != nullptr);
    return buffer.readShort((uint16_t*)value);
}

bool serialize(MemoryBuffer& buffer, int32_t value)
{
    buffer.writeInteger((uint32_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int32_t* value)
{
    assert(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryBuffer& buffer, int64_t value)
{
    buffer.writeWord((uint64_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, int64_t* value)
{
    assert(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}

bool serialize(MemoryBuffer& buffer, float value)
{
    buffer.writeInteger((uint32_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, float* value)
{
    assert(value != nullptr);
    return buffer.readInteger((uint32_t*)value);
}

bool serialize(MemoryBuffer& buffer, double value)
{
    buffer.writeWord((uint64_t*)&value);
    return true;
}

bool deserialize(MemoryBuffer& buffer, double* value)
{
    assert(value != nullptr);
    return buffer.readWord((uint64_t*)value);
}
