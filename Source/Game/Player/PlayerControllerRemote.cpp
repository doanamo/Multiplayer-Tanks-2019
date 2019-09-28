#include "Precompiled.hpp"
#include "PlayerControllerRemote.hpp"

PlayerControllerRemote::PlayerControllerRemote() :
    m_sequenceIndex(0),
    m_acknowledgedIndex(0)
{
}

PlayerControllerRemote::~PlayerControllerRemote()
{
}

void PlayerControllerRemote::consumePlayerCommands()
{
    // Consume queued player commands and save them in command buffer.
    PlayerCommand playerCommand;
    
    while(true)
    {
        playerCommand = popPlayerCommand();

        if(playerCommand != PlayerCommand::Invalid)
        {
#if ENABLE_CONTROLLER_LOG_TRACE
            LOG_TRACE("Consuming player command index %i", playerCommand);
#endif

            pushBufferedCommand(playerCommand);
        }
        else
        {
            break;
        }
    }
}

void PlayerControllerRemote::pushBufferedCommand(PlayerCommand playerCommand)
{
    // Pop command that would overflow buffer.
    if(m_commandBuffer.size() == CommandBufferSize)
    {
        m_commandBuffer.pop_front();
    }

    ASSERT(m_commandBuffer.size() < CommandBufferSize);

    // Push buffered command.
    PlayerCommandEntry commandEntry;
    commandEntry.index = ++m_sequenceIndex;
    commandEntry.command = playerCommand;

    m_commandBuffer.push_back(commandEntry);
}

void PlayerControllerRemote::acknowledgePlayerCommand(PlayerCommandIndex index)
{
    m_acknowledgedIndex = std::max(m_acknowledgedIndex, index);
}

std::vector<PlayerCommandEntry> PlayerControllerRemote::constructPlayerCommandBuffer()
{
    // Remove already acknowledged commands.
    while(!m_commandBuffer.empty())
    {
        const PlayerCommandEntry& entry = m_commandBuffer.front();
        
        if(entry.index <= m_acknowledgedIndex)
        {
            m_commandBuffer.pop_front();
        }
        else
        {
            break;
        }
    }

    // Copy unacknowledged commands into vector.
    std::vector<PlayerCommandEntry> commandArray;

    for(const auto& entry : m_commandBuffer)
    {
        commandArray.push_back(entry);
    }

    return commandArray;
}

bool PlayerControllerRemote::hasUnacknowledgedPlayerCommands() const
{
    ASSERT(m_acknowledgedIndex <= m_sequenceIndex);
    return m_acknowledgedIndex != m_sequenceIndex;
}

bool PlayerControllerRemote::isSerializable() const
{
    return false;
}
