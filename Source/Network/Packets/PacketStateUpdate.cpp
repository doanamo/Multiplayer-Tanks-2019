#include "Precompiled.hpp"
#include "PacketStateUpdate.hpp"

PacketStateUpdate::PacketStateUpdate()
{

}

PacketStateUpdate::~PacketStateUpdate()
{

}

bool PacketStateUpdate::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    return true;
}

bool PacketStateUpdate::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onSerialize(buffer))
        return false;

    return true;
}
