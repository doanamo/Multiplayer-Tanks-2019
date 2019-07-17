#pragma once

#include "Precompiled.hpp"
#include "Game/World/Object.hpp"

// Forward declarations.
class ReplicationBase;

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
    friend ReplicationBase;

    // Protected constructor.
    Replicable();

public:
    // Virtual destructor.
    virtual ~Replicable();

    // Serializes initial replication.
    virtual bool serializeInitialReplication(MemoryStream& stream);

    // Deserializes initial replication.
    virtual bool deserializeInitialReplication(MemoryStream& stream);

    // Serializes reliable tick replication.
    virtual bool serializeReliableTickReplication(MemoryStream& stream);

    // Deserializes reliable tick replication.
    virtual bool deserializeReliableTickReplication(MemoryStream& stream);

    // Serializes unreliable tick replication.
    virtual bool serializeUnreliableTickReplication(MemoryStream& stream);

    // Deserializes unreliable tick replication.
    virtual bool deserializeUnreliableTickReplication(MemoryStream& stream);

    // Gets object's replicable handle.
    ReplicableHandle getReplicableHandle() const;

protected:
    // Serialization methods.
    virtual bool onSerialize(MemoryStream& buffer) const override;
    virtual bool onDeserialize(MemoryStream& buffer) override;

private:
    // Replicable network handle.
    ReplicableHandle m_replicableHandle;
};
