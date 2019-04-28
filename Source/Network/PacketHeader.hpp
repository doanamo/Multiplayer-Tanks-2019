#pragma once

#include "Precompiled.hpp"

class PacketHeader : public Serializable
{
public:
    // Type declarations.
    using PacketType = TypeInfo::IdentifierType;

public:
    PacketHeader();
    ~PacketHeader();

    // Serialization for packet type.
    bool onSerialize(MemoryBuffer& buffer);
    bool onDeserialize(MemoryBuffer& buffer);

public:
    // todo: add crc
    PacketType type;
};
