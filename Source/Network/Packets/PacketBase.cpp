#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

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
