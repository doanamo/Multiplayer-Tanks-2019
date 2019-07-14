#include "Precompiled.hpp"
#include "Replicable.hpp"

Replicable::Replicable()
{
}

Replicable::~Replicable()
{
}

bool Replicable::onReliableReplicationWrite(MemoryStream& stream)
{
    return true;
}

bool Replicable::onReliableReplicationRead(MemoryStream& stream)
{
    return true;
}

bool Replicable::onUnreliableReplicationWrite(MemoryStream& stream)
{
    return true;
}

bool Replicable::onUnreliableReplicationRead(MemoryStream& stream)
{
    return true;
}

ReplicableHandle Replicable::getReplicableHandle() const
{
    return m_replicableHandle;
}
