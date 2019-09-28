#pragma once

#include "Precompiled.hpp"
#include "PlayerControllerHuman.hpp"

// Remote player controller with input history.
class PlayerControllerRemote : public PlayerControllerHuman
{
    TYPE_DECLARE(PlayerControllerRemote, PlayerControllerHuman);

public:
    PlayerControllerRemote();
    ~PlayerControllerRemote();

    // Puts all queued commands into a command buffer.
    void consumePlayerCommands();
    
    // Acknowledged that command index has been received by server.
    void acknowledgePlayerCommand(PlayerCommandIndex index);

    // Constructs an array of unacknowledged player commands.
    std::vector<PlayerCommandEntry> constructPlayerCommandBuffer();

    // Indicates whether player commands should be sent.
    bool hasUnacknowledgedPlayerCommands() const;

    // Returns whether controller should be serializable.
    bool isSerializable() const override;

private:
    // Pushes new command into buffer queue.
    void pushBufferedCommand(PlayerCommand playerCommand);

private:
    // Last processed player command indices.
    PlayerCommandIndex m_sequenceIndex;
    PlayerCommandIndex m_acknowledgedIndex;

    // Buffer of recent player commands.
    static const int CommandBufferSize = 32;
    std::deque<PlayerCommandEntry> m_commandBuffer;
};
