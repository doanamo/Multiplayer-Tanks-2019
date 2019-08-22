#include "Precompiled.hpp"
#include "Tank.hpp"
#include "Projectile.hpp"
#include "Game/World/World.hpp"
#include "System/AssetManager.h"

Tank::Tank() :
    m_movementInput(0.0f, 0.0f)
{
    // Load tank texture.
    m_texture = g_assetManager->loadTexture("Data/Sprites/tank_spritesheet.png");
}

Tank::~Tank()
{
}

void Tank::shootProjectile()
{
    // Create projectile.
    std::unique_ptr<Projectile> projectile(new Projectile());
    projectile->setPosition(m_transform.getPosition() + m_transform.getDirection() * 0.4f);
    projectile->setDirection(m_transform.getDirection());
    getWorld()->addObject(std::move(projectile), "", "Projectiles");
}

void Tank::setMovementInput(sf::Vector2f movement)
{
    m_movementInput = movement;
}

void Tank::onPlayerCommand(PlayerCommand playerCommand)
{
    Super::onPlayerCommand(playerCommand);

    switch(playerCommand)
    {
    case PlayerCommand::Shoot:
        this->shootProjectile();
        break;

    case PlayerCommand::MoveUp:
        this->setMovementInput(sf::Vector2f(0.0f, -1.0f));
        break;

    case PlayerCommand::MoveDown:
        this->setMovementInput(sf::Vector2f(0.0f, 1.0f));
        break;

    case PlayerCommand::MoveLeft:
        this->setMovementInput(sf::Vector2f(-1.0f, 0.0f));
        break;

    case PlayerCommand::MoveRight:
        this->setMovementInput(sf::Vector2f(1.0f, 0.0f));
        break;
    }
}

void Tank::onTick(float timeDelta)
{
    Super::onTick(timeDelta);

    // Move player in direction.
    if(m_movementInput != sf::Vector2f(0.0f, 0.0f))
    {
        // Update transform.
        m_transform.setDirection(m_movementInput);
        m_transform.move(m_movementInput * timeDelta * 4.0f);

        // Reset movement input.
        m_movementInput = sf::Vector2f(0.0f, 0.0f);
    }
}

void Tank::onDraw(float timeAlpha)
{
    Super::onDraw(timeAlpha);

    // Draw tank.
    m_tankSprite.setSize(sf::Vector2f(1.0f, 1.0f));
    m_tankSprite.setOrigin(m_tankSprite.getSize() / 2.0f);
    m_tankSprite.setPosition(m_transform.getPosition(timeAlpha));
    m_tankSprite.setTexture(m_texture.get());

    if(m_transform.getRotation() == 0.0f)
    {
        m_tankSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
    else if(m_transform.getRotation() == 90.0f)
    {
        m_tankSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
    }
    else if(m_transform.getRotation() == 180.0f)
    {
        m_tankSprite.setTextureRect(sf::IntRect(0, 96, 32, 32));
    }
    else if(m_transform.getRotation() == 270.0f)
    {
        m_tankSprite.setTextureRect(sf::IntRect(0, 64, 32, 32));
    }

    g_renderTarget->draw(m_tankSprite);
}

sf::Vector2f Tank::getPosition(float alpha)
{
    return m_transform.getPosition(alpha);
}

bool Tank::onSerialize(MemoryStream& buffer) const
{
    if(!Super::onSerialize(buffer))
        return false;

    if(!serialize(buffer, m_movementInput))
        return false;

    return true;
}

bool Tank::onDeserialize(MemoryStream& buffer)
{
    if(!Super::onDeserialize(buffer))
        return false;

    if(!deserialize(buffer, &m_movementInput))
        return false;

    return true;
}

bool Tank::serializeInitialReplication(MemoryStream& stream)
{
    if(!serialize(stream, m_transform))
        return false;

    return true;
}

bool Tank::deserializeInitialReplication(MemoryStream& stream)
{
    if(!deserialize(stream, &m_transform))
        return false;

    return true;
}

bool Tank::serializeUnreliableTickReplication(MemoryStream& stream)
{
    if(!serialize(stream, m_transform.m_currentPosition))
        return false;

    if(!serialize(stream, m_transform.m_currentRotation))
        return false;

    return true;
}

bool Tank::deserializeUnreliableTickReplication(MemoryStream& stream)
{
    // Reset interpolation now in pre tick instead of during tick.
    // Otherwise transform read here will be treated as previous and interpolation will not work properly.
    if(m_resetInterpolation)
    {
        m_transform.resetInterpolation();
        m_resetInterpolation = false;
    }

    // Read transform update.
    if(!deserialize(stream, &m_transform.m_currentPosition))
        return false;

    if(!deserialize(stream, &m_transform.m_currentRotation))
        return false;

    return true;
}
