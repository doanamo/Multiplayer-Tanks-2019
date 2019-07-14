#include "Precompiled.hpp"
#include "Network/Packets/PacketServerSnapshot.hpp"
#include "Game/GameInstance.hpp"

PacketServerSnapshot::PacketServerSnapshot()
{
}

PacketServerSnapshot::~PacketServerSnapshot()
{
}

bool PacketServerSnapshot::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, serializedGameInstance))
        return false;

    return true;
}

bool PacketServerSnapshot::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &serializedGameInstance))
        return false;

    return true;
}
