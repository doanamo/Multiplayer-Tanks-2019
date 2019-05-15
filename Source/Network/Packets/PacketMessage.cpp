#include "Precompiled.hpp"
#include "PacketMessage.hpp"

PacketMessage::PacketMessage()
{
}

PacketMessage::~PacketMessage()
{
}

bool PacketMessage::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, text))
        return false;

    return true;
}

bool PacketMessage::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &text))
        return false;

    return true;
}
