#include "Precompiled.hpp"
#include "Network/PacketContainer.hpp"

static const char MagicValue[] = "TanksXD";
static const int MagicSize = staticArraySize(MagicValue);

PacketContainer::PacketContainer() :
    packetType(stringHash(""))
{
}

PacketContainer::~PacketContainer()
{
}

bool PacketContainer::onSerialize(MemoryBuffer& buffer)
{
    for(char magic : MagicValue)
    {
        if(!serialize(buffer, magic))
            return false;
    }

    uint32_t crc = 0;
    crc = calculateCRC32(crc, (const char*)&packetType, sizeof(packetType));
    crc = calculateCRC32(crc, packetBuffer.data(), packetBuffer.size());

    if(!serialize(buffer, crc))
        return false;

    if(!serialize(buffer, packetType))
        return false;

    if(!serialize(buffer, packetBuffer))
        return false;

    return true;
}

bool PacketContainer::onDeserialize(MemoryBuffer& buffer)
{
    for(char magic : MagicValue)
    {
        char character;
        if(!deserialize(buffer, &character))
            return false;

        if(character != magic)
        {
            LOG_WARNING("Could not deserialize packet data due to invalid magic!");
            return false;
        }
    }

    uint32_t crc;
    if(!deserialize(buffer, &crc))
        return false;

    if(!deserialize(buffer, &packetType))
        return false;

    if(!deserialize(buffer, &packetBuffer))
        return false;

    uint32_t verify = 0;
    verify = calculateCRC32(verify, (const char*)&packetType, sizeof(packetType));
    verify = calculateCRC32(verify, packetBuffer.data(), packetBuffer.size());

    if(verify != crc)
    {
        LOG_WARNING("Could not deserialize packet data due to invalid CRC!");
        //return false;
    }

    return true;
}
