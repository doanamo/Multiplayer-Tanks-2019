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
    // Write packet type.
    if(!serialize(buffer, getTypeInfo().getIdentifier()))
        return false;

    return true;
}

bool PacketBase::onDeserialize(MemoryStream& buffer)
{
    // Packet type is not deserialized here.
    // Received must read the first uint32 themselves and then allocate instance.
    // #todo: Look for a better solution which would instantiate object automatically on deserialization.

    return true;
}
