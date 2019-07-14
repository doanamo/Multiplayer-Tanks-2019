#pragma once

#include "Precompiled.hpp"
#include "Replicable.hpp"

// Replication command.
struct ReplicationCommand
{
    // Type of replication.
    enum ReplicationType
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
