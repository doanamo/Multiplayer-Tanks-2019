#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

PacketBase::PacketBase()
{
}

PacketBase::~PacketBase()
{
}

bool PacketBase::onSerialize(MemoryBuffer& buffer) const
{
    return true;
}

bool PacketBase::onDeserialize(MemoryBuffer& buffer)
{
    return true;
}
