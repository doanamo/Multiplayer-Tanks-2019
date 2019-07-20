#pragma once

#include "Precompiled.hpp"
#include "Replicable.hpp"

// Replication command.
struct ReplicationCommand
{
    // Type of replication.
    enum ReplicationType : uint8_t
    {
        Create,
        Update,
        Destroy,
    };

    ReplicationType type;

    // Shared replication handle.
    ReplicableHandle handle;

    // Replication data for specified handle.
    MemoryStream data;
};

// Serialization functions.
bool serialize(MemoryStream& stream, const ReplicationCommand& command);
bool deserialize(MemoryStream& stream, ReplicationCommand* command);
