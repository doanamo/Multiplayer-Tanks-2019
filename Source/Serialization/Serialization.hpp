#pragma once

#include "Precompiled.hpp"
#include "Serialization/MemoryBuffer.hpp"
#include "Serialization/Serializable.hpp"

bool serialize(MemoryBuffer& buffer, Serializable& serializable);
bool deserialize(MemoryBuffer& buffer, Serializable* serializable);

bool serialize(MemoryBuffer& buffer, uint8_t value);
bool deserialize(MemoryBuffer& buffer, uint8_t* value);
bool serialize(MemoryBuffer& buffer, uint16_t value);
bool deserialize(MemoryBuffer& buffer, uint16_t* value);
bool serialize(MemoryBuffer& buffer, uint32_t value);
bool deserialize(MemoryBuffer& buffer, uint32_t* value);
bool serialize(MemoryBuffer& buffer, uint64_t value);
bool deserialize(MemoryBuffer& buffer, uint64_t* value);

bool serialize(MemoryBuffer& buffer, int8_t value);
bool deserialize(MemoryBuffer& buffer, int8_t* value);
bool serialize(MemoryBuffer& buffer, int16_t value);
bool deserialize(MemoryBuffer& buffer, int16_t* value);
bool serialize(MemoryBuffer& buffer, int32_t value);
bool deserialize(MemoryBuffer& buffer, int32_t* value);
bool serialize(MemoryBuffer& buffer, int64_t value);
bool deserialize(MemoryBuffer& buffer, int64_t* value);

bool serialize(MemoryBuffer& buffer, float value);
bool deserialize(MemoryBuffer& buffer, float* value);
bool serialize(MemoryBuffer& buffer, double value);
bool deserialize(MemoryBuffer& buffer, double* value);
