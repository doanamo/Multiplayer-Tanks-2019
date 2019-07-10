#include "Precompiled.hpp"
#include "Network/Packets/PacketHeader.hpp"

static const char MagicValue[] = "TanksXD";
static const std::size_t MagicSize = staticArraySize(MagicValue);

PacketHeader::PacketHeader() :
    checksum(0),
    sequenceIndex(0),
    previousReliableIndex(0),
    acknowledgmentIndex(0),
    transportMethod(0),
    transportExtra(0)
{
}

PacketHeader::~PacketHeader()
{
}

uint32_t PacketHeader::calculateChecksum(const char* data, std::size_t size) const
{
    uint32_t crc = 0;
    crc = calculateCRC32(crc, (const char*)&sequenceIndex, sizeof(sequenceIndex));
    crc = calculateCRC32(crc, (const char*)&previousReliableIndex, sizeof(previousReliableIndex));
    crc = calculateCRC32(crc, (const char*)&acknowledgmentIndex, sizeof(acknowledgmentIndex));
    crc = calculateCRC32(crc, (const char*)&transportMethod, sizeof(transportMethod));
    crc = calculateCRC32(crc, (const char*)&transportExtra, sizeof(transportExtra));
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

    if(!serialize(buffer, checksum))
        return false;

    if(!serialize(buffer, sequenceIndex))
        return false;

    if(!serialize(buffer, previousReliableIndex))
        return false;

    if(!serialize(buffer, acknowledgmentIndex))
        return false;

    if(!serialize(buffer, transportMethod))
        return false;

    if(!serialize(buffer, transportExtra))
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

    if(!deserialize(buffer, &checksum))
        return false;

    if(!deserialize(buffer, &sequenceIndex))
        return false;

    if(!deserialize(buffer, &previousReliableIndex))
        return false;

    if(!deserialize(buffer, &acknowledgmentIndex))
        return false;

    if(!deserialize(buffer, &transportMethod))
        return false;

    if(!deserialize(buffer, &transportExtra))
        return false;

    return true;
}
