#include "Precompiled.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"
#include "Game/World.hpp"
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
    Projectile* projectile = new Projectile();
    projectile->setPosition(m_transform.getPosition() + m_transform.getDirection() * 0.4f);
    projectile->setDirection(m_transform.getDirection());
    getWorld()->addObject(projectile, "", "Projectiles");
}

void Tank::setMovementInput(sf::Vector2f movement)
{
    m_movementInput = movement;
}

void Tank::onTick(float timeDelta)
{
    Object::onTick(timeDelta);

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
    Object::onDraw(timeAlpha);

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

    g_render->draw(m_tankSprite);
}

sf::Vector2f Tank::getPosition(float alpha)
{
    return m_transform.getPosition(alpha);
}

bool Tank::onSerialize(MemoryBuffer& buffer)
{
    Object::onSerialize(buffer);

    if(!serialize(buffer, m_movementInput))
        return false;

    return true;
}

bool Tank::onDeserialize(MemoryBuffer& buffer)
{
    Object::onDeserialize(buffer);

    if(!deserialize(buffer, &m_movementInput))
        return false;

    return true;
}
