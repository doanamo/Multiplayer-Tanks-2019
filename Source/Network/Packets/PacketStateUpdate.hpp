#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"

class PacketStateUpdate : public PacketBase
{
    TYPE_DECLARE(PacketStateUpdate, PacketBase);

public:
    PacketStateUpdate();
    ~PacketStateUpdate();

    bool onSerialize(MemoryStream& buffer) const;
    bool onDeserialize(MemoryStream& buffer);

public:
    // Last processed tick frame.
    // Used too determine last reliable data processed.

    // Reliable data delta.
    // Spawning and destroying objects.
    // Game state changes and events.

    // Unreliable data update.
    // May not be sent if reliable data needs space.
    // Position updates.
};
