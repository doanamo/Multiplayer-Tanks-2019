#pragma once

#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

// Client's player controller on server's side.
class PlayerControllerClient : public PlayerControllerBase
{
    TYPE_DECLARE(PlayerControllerClient, PlayerControllerBase);

public:
    PlayerControllerClient();
    ~PlayerControllerClient();

    // Processes received player command buffer.
    void processPlayerCommandBuffer(const std::vector<PlayerCommandEntry>& commandBuffer);

    // Returns last received player index.
    PlayerCommandIndex getAcknowledgedIndex() const;

    // Returns whether controller should be serializable.
    bool isSerializable() const override;

private:
    // Last received player command index;
    PlayerCommandIndex m_acknowledgedIndex;
};
