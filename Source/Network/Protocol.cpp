#include "Precompiled.hpp"
#include "Protocol.hpp"

static const char MagicValue[] = "TanksXD";
static const int MagicSize = staticArraySize(MagicValue);

PacketHeader::PacketHeader()
{
    type = stringHash("");
}

PacketHeader::~PacketHeader()
{
}

bool PacketHeader::onSerialize(MemoryBuffer& buffer)
{
    for(char magic : MagicValue)
    {
        if(!serialize(buffer, magic))
            return false;
    }

    if(!serialize(buffer, type))
        return false;

    return true;
}

bool PacketHeader::onDeserialize(MemoryBuffer& buffer)
{
    for(char magic : MagicValue)
    {
        char character = '\0';
        if(!deserialize(buffer, &character))
            return false;

        if(character != magic)
            return false;
    }

    if(!deserialize(buffer, &type))
        return false;

    return true;
}

PacketBase::PacketBase()
{
}

PacketBase::~PacketBase()
{
}

bool PacketBase::onSerialize(MemoryBuffer& buffer)
{
    return true;
}

bool PacketBase::onDeserialize(MemoryBuffer& buffer)
{
    return true;
}

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
