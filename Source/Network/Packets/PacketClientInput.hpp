#pragma once

#include "Precompiled.hpp"
#include "PacketBase.hpp"
#include "Game/Player/Player.hpp"
#include "Game/Player/PlayerCommand.hpp"

// Client input packet.
class PacketClientInput : public PacketBase
{
    TYPE_DECLARE(PacketClientInput, PacketBase);

public:
    PacketClientInput();
    ~PacketClientInput();

protected:
    // Packet serialization methods.
    virtual bool onSerialize(MemoryStream& stream) const override final;
    virtual bool onDeserialize(MemoryStream& stream) override final;

public:
    // Player handle.
    PlayerHandle playerHandle;

    // Queued player commands.
    std::vector<PlayerCommandEntry> playerCommands; // make entry serializable???
};
