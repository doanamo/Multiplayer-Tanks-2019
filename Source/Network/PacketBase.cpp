#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

PacketBase::PacketBase()
{
}

PacketBase::~PacketBase()
{
}

bool PacketBase::onSerialize(MemoryStream& buffer) const
{
    return true;
}

bool PacketBase::onDeserialize(MemoryStream& buffer)
{
    return true;
}
