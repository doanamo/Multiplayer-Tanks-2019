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
bool serialize(MemoryBuffer& buffer, Serializable* data)
{
    assert(data != nullptr);
    return data->serialize(buffer);
}

template<>
bool deserialize(MemoryBuffer& buffer, Serializable* data)
{
    assert(data != nullptr);
    return data->deserialize(buffer);
}
