#include "Precompiled.hpp"
#include "Replicable.hpp"

Replicable::Replicable()
{
}

Replicable::~Replicable()
{
}

bool Replicable::serializeInitialReplication(MemoryStream& stream)
{
    return true;
}

bool Replicable::deserializeInitialReplication(MemoryStream& stream)
{
    return true;
}

bool Replicable::serializeReliableTickReplication(MemoryStream& stream)
{
    return true;
}

bool Replicable::deserializeReliableTickReplication(MemoryStream& stream)
{
    return true;
}

bool Replicable::serializeUnreliableTickReplication(MemoryStream& stream)
{
    return true;
}

bool Replicable::deserializeUnreliableTickReplication(MemoryStream& stream)
{
    return true;
}

ReplicableHandle Replicable::getReplicableHandle() const
{
    return m_replicableHandle;
}

bool Replicable::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    // Serialize networked handle.
    if(!serialize(buffer, m_replicableHandle))
        return false;

    return true;
}

bool Replicable::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    // Serialize networked handle.
    if(!deserialize(buffer, &m_replicableHandle))
        return false;

    return true;
}
