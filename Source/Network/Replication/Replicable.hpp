#pragma once

#include "Precompiled.hpp"
#include "Game/World/Object.hpp"

// Forward declarations.
class Replication;

// Replicable entry.
struct ReplicableEntry
{
    ReplicableEntry() :
        objectHandle(),
        object(nullptr)
    {
    }

    ObjectHandle objectHandle;
    Object* object;
};

using ReplicableHandle = Handle<ReplicableEntry>;

// Replicable base class.
class Replicable : public Object
{
    TYPE_DECLARE(Replicable, Object);

protected:
    // Allow replication system to access this class.
    friend Replication;

    // Protected constructor.
    Replicable();

public:
    // Virtual destructor.
    virtual ~Replicable();

    // Serializes reliable replication data.
    virtual bool onReliableReplicationWrite(MemoryStream& stream);

    // Deserializes reliable replication data.
    virtual bool onReliableReplicationRead(MemoryStream& stream);

    // Serializes unreliable replication data.
    virtual bool onUnreliableReplicationWrite(MemoryStream& stream);

    // Deserializes unreliable replication data.
    virtual bool onUnreliableReplicationRead(MemoryStream& stream);

    // Gets object's replicable handle.
    ReplicableHandle getReplicableHandle() const;

private:
    // Replicable network handle.
    ReplicableHandle m_replicableHandle;
};
