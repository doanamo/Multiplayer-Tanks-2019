#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"

class PacketServerUpdate : public PacketBase
{
    TYPE_DECLARE(PacketServerUpdate, PacketBase);

public:
    PacketServerUpdate();
    ~PacketServerUpdate();

    bool onSerialize(MemoryStream& buffer) const;
    bool onDeserialize(MemoryStream& buffer);

public:
    // Last processed tick frame.
    // Used too determine last reliable data processed.
    // We may actually not be needing this as packets are always ordered.
    uint64_t tickFrame;

    // Reliable data delta.
    // Spawning and destroying objects.
    // Game state changes and events.

    // Unreliable data update.
    // May not be sent if reliable data needs space.
    // Position updates.
};
