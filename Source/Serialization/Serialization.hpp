#pragma once

#include "Precompiled.hpp"
#include "Serialization/MemoryStream.hpp"
#include "Serialization/Serializable.hpp"

bool serialize(MemoryStream& buffer, const Serializable& serializable);
bool deserialize(MemoryStream& buffer, Serializable* serializable);

bool serialize(MemoryStream& buffer, uint8_t value);
bool deserialize(MemoryStream& buffer, uint8_t* value);
bool serialize(MemoryStream& buffer, uint16_t value);
bool deserialize(MemoryStream& buffer, uint16_t* value);
bool serialize(MemoryStream& buffer, uint32_t value);
bool deserialize(MemoryStream& buffer, uint32_t* value);
bool serialize(MemoryStream& buffer, uint64_t value);
bool deserialize(MemoryStream& buffer, uint64_t* value);

bool serialize(MemoryStream& buffer, int8_t value);
bool deserialize(MemoryStream& buffer, int8_t* value);
bool serialize(MemoryStream& buffer, int16_t value);
bool deserialize(MemoryStream& buffer, int16_t* value);
bool serialize(MemoryStream& buffer, int32_t value);
bool deserialize(MemoryStream& buffer, int32_t* value);
bool serialize(MemoryStream& buffer, int64_t value);
bool deserialize(MemoryStream& buffer, int64_t* value);

bool serialize(MemoryStream& buffer, char value);
bool deserialize(MemoryStream& buffer, char* value);
bool serialize(MemoryStream& buffer, float value);
bool deserialize(MemoryStream& buffer, float* value);
bool serialize(MemoryStream& buffer, double value);
bool deserialize(MemoryStream& buffer, double* value);

bool serialize(MemoryStream& buffer, const sf::Vector2f& value);
bool deserialize(MemoryStream& buffer, sf::Vector2f* value);

bool serialize(MemoryStream& buffer, const std::string& value);
bool deserialize(MemoryStream& buffer, std::string* value);
