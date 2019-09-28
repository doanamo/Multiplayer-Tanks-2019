#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"
#include "Network/Replication/ReplicationCommand.hpp"
#include "Game/Player/PlayerCommand.hpp"

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

    // Acknowledged player command.
    PlayerCommandIndex acknowledgedPlayerCommand;

    // List of replication commands.
    std::vector<ReplicationCommand> replicationCommands;
};
