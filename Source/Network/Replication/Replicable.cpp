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
