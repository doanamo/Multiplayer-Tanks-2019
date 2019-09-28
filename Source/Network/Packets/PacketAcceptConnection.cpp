#include "Precompiled.hpp"
#include "PacketAcceptConnection.hpp"

PacketAcceptConnection::PacketAcceptConnection()
{
}

PacketAcceptConnection::~PacketAcceptConnection()
{
}

bool PacketAcceptConnection::onSerialize(MemoryStream& buffer) const
{
    ASSERT(playerHandle.isValid(), "Excepted valid player handle to be set!");

    if(!serialize(buffer, playerHandle))
        return false;

    return true;
}

bool PacketAcceptConnection::onDeserialize(MemoryStream& buffer)
{
    if(!deserialize(buffer, &playerHandle))
        return false;

    return true;
}
