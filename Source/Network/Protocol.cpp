#include "Precompiled.hpp"
#include "Protocol.hpp"

PacketHeader::PacketHeader() :
    type(PacketType::Invalid)
{
    strcpy(&magic[0], "TanksXD");
}

bool serialize(MemoryBuffer& buffer, const PacketHeader& header)
{
    // Write packet magic.
    for(char c : header.magic)
    {
        if(!serialize(buffer, c))
            return false;
    }

    // Write packet type.
    if(!serialize(buffer, static_cast<std::underlying_type_t<PacketType>>(header.type)))
        return false;

    return true;
}

bool deserialize(MemoryBuffer& buffer, PacketHeader* header)
{
    ASSERT(header != nullptr);

    // Read packet magic.
    for(char& c : header->magic)
    {
        if(!deserialize(buffer, &c))
            return false;
    }

    // Read packet type.
    if(!deserialize(buffer, reinterpret_cast<std::underlying_type_t<PacketType>*>(&header->type)))
        return false;

    return true;
}
