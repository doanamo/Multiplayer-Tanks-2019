#include "Precompiled.hpp"
#include "PacketAcceptConnection.hpp"

PacketAcceptConnection::PacketAcceptConnection() :
    playerIndex(0)
{
}

PacketAcceptConnection::~PacketAcceptConnection()
{
}

bool PacketAcceptConnection::onSerialize(MemoryStream& buffer) const
{
    if(!serialize(buffer, playerIndex))
        return false;

    return true;
}

bool PacketAcceptConnection::onDeserialize(MemoryStream& buffer)
{
    if(!deserialize(buffer, &playerIndex))
        return false;

    return true;
}
