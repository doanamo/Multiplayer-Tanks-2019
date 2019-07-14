#include "Precompiled.hpp"
#include "PacketServerUpdate.hpp"

PacketServerUpdate::PacketServerUpdate()
{

}

PacketServerUpdate::~PacketServerUpdate()
{

}

bool PacketServerUpdate::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    return true;
}

bool PacketServerUpdate::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onSerialize(buffer))
        return false;

    return true;
}
