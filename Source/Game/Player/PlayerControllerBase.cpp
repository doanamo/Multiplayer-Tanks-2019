#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

#if ENABLE_PLAYER_CONTROLLER_LOG_TRACE
    #define LOG_PLAYER_CONTROLLER_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_PLAYER_CONTROLLER_TRACE(format, ...)
#endif

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
    LOG_PLAYER_CONTROLLER_TRACE("Ticking player controller");
}

void PlayerControllerBase::pushPlayerCommand(PlayerCommand playerCommand)
{
    LOG_PLAYER_CONTROLLER_TRACE("Pushing player command index %i", (int)playerCommand);

    m_playerCommands.push(playerCommand);
}

PlayerCommand PlayerControllerBase::popPlayerCommand()
{
    PlayerCommand playerCommand = PlayerCommand::Invalid;

    if(!m_playerCommands.empty())
    {
        playerCommand = m_playerCommands.front();

        LOG_PLAYER_CONTROLLER_TRACE("Popping player command index %i", (int)playerCommand);

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

bool PlayerControllerBase::isSerializable() const
{
    return true;
}

bool PlayerControllerBase::onSerialize(MemoryStream& buffer) const
{
    ASSERT(m_playerCommands.empty(), "Player command queue is expeted to be empty during serialization!");

    // Serialize controlled object handle.
    if(!serialize(buffer, m_controlledObject))
        return false;

    return true;
}

bool PlayerControllerBase::onDeserialize(MemoryStream& buffer)
{
    // Deserialize controlled object handle.
    if(!deserialize(buffer, &m_controlledObject))
        return false;

    return true;
}
