#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

PlayerControllerBase::PlayerControllerBase()
{
}

PlayerControllerBase::~PlayerControllerBase()
{
}

bool PlayerControllerBase::handleEvent(const sf::Event& event)
{
    return true;
}

void PlayerControllerBase::tick(float timeDelta)
{
}

void PlayerControllerBase::pushPlayerCommand(PlayerCommand playerCommand)
{
    m_playerCommands.push(playerCommand);
}

PlayerCommand PlayerControllerBase::popPlayerCommand()
{
    PlayerCommand playerCommand = PlayerCommand::Invalid;

    if(!m_playerCommands.empty())
    {
        playerCommand = m_playerCommands.front();
        m_playerCommands.pop();
    }

    return playerCommand;
}

void PlayerControllerBase::setControlledObject(const ObjectHandle& objectHandle)
{
    m_controlledObject = objectHandle;

    // Clear player command queue when controlled object is changed.
    m_playerCommands = std::queue<PlayerCommand>();
}

const ObjectHandle& PlayerControllerBase::getControlledObject() const
{
    return m_controlledObject;
}
