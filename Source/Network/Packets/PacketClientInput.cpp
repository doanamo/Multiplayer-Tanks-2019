#include "Precompiled.hpp"
#include "PacketClientInput.hpp"

PacketClientInput::PacketClientInput()
{
}

PacketClientInput::~PacketClientInput()
{
}

bool PacketClientInput::onSerialize(MemoryStream& stream) const
{
    // Serialize player handle.
    ASSERT(playerHandle.isValid(), "Excepted valid player handle to be set!");

    if(!serialize(stream, playerHandle))
        return false;

    // Serialize number of player commands.
    std::size_t playerCommandCount = playerCommands.size();
    if(!serialize(stream, playerCommandCount))
        return false;
    
    // Write each player command.
    for(std::size_t i = 0; i < playerCommandCount; ++i)
    {
        PlayerCommandEntry playerCommand = playerCommands[i];

        if(!serialize(stream, playerCommand.index))
            return false;

        using enumType = std::underlying_type<PlayerCommand>::type;
        if(!serialize(stream, (enumType)playerCommand.command))
            return false;
    }

    return true;
}

bool PacketClientInput::onDeserialize(MemoryStream& stream)
{
    // Deserialize player handle.
    if(!deserialize(stream, &playerHandle))
        return false;

    // Deserialize number of player commands.
    std::size_t playerCommandCount;
    if(!deserialize(stream, &playerCommandCount))
        return false;

    // Read each player command.
    for(std::size_t i = 0; i < playerCommandCount; ++i)
    {
        PlayerCommandEntry playerCommand;

        if(!deserialize(stream, &playerCommand.index))
            return false;

        using enumType = std::underlying_type<PlayerCommand>::type;
        if(!deserialize(stream, (enumType*)&playerCommand.command))
            return false;

        playerCommands.push_back(playerCommand);
    }

    return true;
}
