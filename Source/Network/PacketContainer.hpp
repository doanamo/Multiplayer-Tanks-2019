#pragma once

#include "Precompiled.hpp"

class PacketContainer : public Serializable
{
public:
    // Type declarations.
    using PacketType = TypeInfo::IdentifierType;

public:
    PacketContainer();
    ~PacketContainer();

    // Serialization for packet type.
    bool onSerialize(MemoryBuffer& buffer) const override;
    bool onDeserialize(MemoryBuffer& buffer) override;

public:
    PacketType packetType;
    MemoryBuffer packetBuffer;
};
