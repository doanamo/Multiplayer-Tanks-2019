#include "Precompiled.hpp"
#include "PacketServerUpdate.hpp"

PacketServerUpdate::PacketServerUpdate() :
    tickFrame(0),
    acknowledgedPlayerCommand(0)
{
}

PacketServerUpdate::~PacketServerUpdate()
{
}

bool PacketServerUpdate::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, tickFrame))
        return false;

    if(!serialize(buffer, acknowledgedPlayerCommand))
        return false;

    if(!serialize(buffer, (uint32_t)replicationCommands.size()))
        return false;

    for(const auto& replicationCommand : replicationCommands)
    {
        if(!serialize(buffer, replicationCommand))
            return false;
    }

    return true;
}

bool PacketServerUpdate::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &tickFrame))
        return false;

    if(!deserialize(buffer, &acknowledgedPlayerCommand))
        return false;

    uint32_t replicationCommandCount = 0;
    if(!deserialize(buffer, &replicationCommandCount))
        return false;

    for(uint32_t i = 0; i < replicationCommandCount; ++i)
    {
        ReplicationCommand replicationCommand;
        if(!deserialize(buffer, &replicationCommand))
            return false;

        replicationCommands.push_back(replicationCommand);
    }

    return true;
}
