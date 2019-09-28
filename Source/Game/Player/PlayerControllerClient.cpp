#include "Precompiled.hpp"
#include "PlayerControllerClient.hpp"

PlayerControllerClient::PlayerControllerClient() :
    m_acknowledgedIndex(0)
{
}

PlayerControllerClient::~PlayerControllerClient()
{
}

void PlayerControllerClient::processPlayerCommandBuffer(const std::vector<PlayerCommandEntry>& commandBuffer)
{
    // Process received command buffers.
    for(auto commandEntry : commandBuffer)
    {
        if(commandEntry.index > m_acknowledgedIndex)
        {
            pushPlayerCommand(commandEntry.command);
            m_acknowledgedIndex = commandEntry.index;
        }
    }
}

PlayerCommandIndex PlayerControllerClient::getAcknowledgedIndex() const
{
    return m_acknowledgedIndex;
}

bool PlayerControllerClient::isSerializable() const
{
    return false;
}
