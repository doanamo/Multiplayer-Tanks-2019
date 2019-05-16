#include "Precompiled.hpp"
#include "Network/Packets/PacketStateSave.hpp"
#include "Game/GameInstance.hpp"

PacketStateSave::PacketStateSave()
{
}

PacketStateSave::~PacketStateSave()
{
}

bool PacketStateSave::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, serializedGameInstance))
        return false;

    return true;
}

bool PacketStateSave::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &serializedGameInstance))
        return false;

    return true;
}
