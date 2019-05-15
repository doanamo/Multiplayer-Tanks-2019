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

    // Calculates packet CRC.
    uint32_t calculateCRC(const char* data, std::size_t size);

    // Serialization for packet type.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    uint32_t packetCRC;
    std::size_t packetSize;
    PacketType packetType;
};
