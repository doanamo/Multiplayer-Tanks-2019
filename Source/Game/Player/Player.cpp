#include "Precompiled.hpp"
#include "Player.hpp"
#include "PlayerControllerBase.hpp"

Player::Player()
{
}
 
Player::~Player()
{
}

void Player::setPlayerHandle(PlayerHandle handle)
{
    ASSERT(!m_playerHandle.isValid(), "Player handle has already been set before!");
    m_playerHandle = handle;
}

void Player::setPlayerController(std::unique_ptr<PlayerControllerBase>&& playerController)
{
    m_playerController = std::move(playerController);
}

PlayerHandle Player::getPlayerHandle() const
{
    ASSERT(m_playerHandle.isValid(), "Player handle should already be set!");
    return m_playerHandle;
}

PlayerControllerBase* Player::getPlayerController()
{
    return m_playerController.get();
}

bool Player::onSerialize(MemoryStream& stream) const
{
    // Determine if controller should be serialized.
    bool serializeController = m_playerController && m_playerController->isSerializable();

    // Serialize controller type identifier.
    TypeInfo::IdentifierType controllerType = TypeInfo::InvalidIdentifier;

    if(serializeController)
    {
        controllerType = getTypeIdentifier(*m_playerController);
    }

    if(!serialize(stream, controllerType))
        return false;

    // Serialize controller object.
    if(serializeController)
    {
        if(!serialize(stream, *m_playerController))
            return false;
    }

    return true;
}

bool Player::onDeserialize(MemoryStream& stream)
{
    // Deserialize controller type identifier.
    TypeInfo::IdentifierType controllerType;
    if(!deserialize(stream, &controllerType))
        return false;

    // Check if type identifier is valid (otherwise controller did not exist).
    if(controllerType != TypeInfo::InvalidIdentifier)
    {
        // Create controller instance.
        std::unique_ptr<PlayerControllerBase> playerController(PlayerControllerBase::create(controllerType));
        ASSERT(playerController, "Failed to created object instance from known type!");

        // Deserialize data into controller instance.
        if(!deserialize(stream, playerController.get()))
            return false;
        
        // Set deserialized player controller.
        this->setPlayerController(std::move(playerController));
    }

    return true;
}
