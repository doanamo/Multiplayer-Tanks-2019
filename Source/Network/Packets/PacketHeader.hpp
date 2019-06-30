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

    // Calculates packet CRC checksum.
    uint32_t calculateChecksum(const char* data, std::size_t size) const;

    // Serialization for packet type.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

public:
    // Packet CRC checksum.
    uint32_t checksum;

    // Packet sequence index.
    uint32_t sequenceIndex;

    // Packet acknowledgment index.
    uint32_t acknowledgmentIndex;

    // Packet transform method.
    uint32_t transformMethod;

    // Extra field used for specified transform method.
    uint32_t transformExtra;
};
