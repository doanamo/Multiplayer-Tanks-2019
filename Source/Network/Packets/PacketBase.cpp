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
    // #todo: Look for a better solution which would instantiate object automatically on deserialization.

    return true;
}
