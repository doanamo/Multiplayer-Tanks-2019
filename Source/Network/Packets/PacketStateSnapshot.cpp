#include "Precompiled.hpp"
#include "Network/Packets/PacketStateSnapshot.hpp"
#include "Game/GameInstance.hpp"

PacketStateSnapshot::PacketStateSnapshot()
{
}

PacketStateSnapshot::~PacketStateSnapshot()
{
}

bool PacketStateSnapshot::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, serializedGameInstance))
        return false;

    return true;
}

bool PacketStateSnapshot::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &serializedGameInstance))
        return false;

    return true;
}
