#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"
#include "Network/Replication/ReplicationCommand.hpp"

class PacketServerUpdate : public PacketBase
{
    TYPE_DECLARE(PacketServerUpdate, PacketBase);

public:
    PacketServerUpdate();
    ~PacketServerUpdate();

    bool onSerialize(MemoryStream& buffer) const;
    bool onDeserialize(MemoryStream& buffer);

public:
    // Last processed tick frame.
    uint64_t tickFrame;

    // List of replication commands.
    std::vector<ReplicationCommand> replicationCommands;
};
