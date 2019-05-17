#include "Precompiled.hpp"
#include "Network/Packets/PacketHeader.hpp"

static const char MagicValue[] = "TanksXD";
static const std::size_t MagicSize = staticArraySize(MagicValue);

PacketHeader::PacketHeader() :
    packetCRC(0),
    packetSize(0),
    packetType(stringHash(""))
{
}

PacketHeader::~PacketHeader()
{
}

uint32_t PacketHeader::calculateCRC(const char* data, std::size_t size)
{
    ASSERT(packetSize != 0, "Packet size should be set before calculating CRC!");
    ASSERT(packetType != stringHash(""), "Packet type should be set before calculating CRC!");

    uint32_t crc = 0;
    crc = calculateCRC32(crc, (const char*)&packetSize, sizeof(packetSize));
    crc = calculateCRC32(crc, (const char*)&packetType, sizeof(packetType));
    crc = calculateCRC32(crc, data, size);
    return crc;
}

bool PacketHeader::onSerialize(MemoryStream& buffer) const
{
    for(char magic : MagicValue)
    {
        if(!serialize(buffer, magic))
            return false;
    }

    if(!serialize(buffer, packetCRC))
        return false;

    if(!serialize(buffer, packetSize))
        return false;

    if(!serialize(buffer, packetType))
        return false;

    return true;
}

bool PacketHeader::onDeserialize(MemoryStream& buffer)
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

    if(!deserialize(buffer, &packetCRC))
        return false;

    if(!deserialize(buffer, &packetSize))
        return false;

    if(!deserialize(buffer, &packetType))
        return false;

    return true;
}
