#include "Precompiled.hpp"
#include "Network/PacketMessage.hpp"

PacketMessage::PacketMessage()
{
}

PacketMessage::~PacketMessage()
{
}

bool PacketMessage::onSerialize(MemoryBuffer& buffer)
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, text))
        return false;

    return true;
}

bool PacketMessage::onDeserialize(MemoryBuffer& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &text))
        return false;

    return true;
}
