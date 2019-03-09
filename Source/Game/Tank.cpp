#include "Precompiled.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"
#include "Game/World.hpp"
#include "System/AssetManager.h"

Tank::Tank() :
    m_currentPosition(0.0f, 0.0f),
    m_previousPosition(0.0f, 0.0f),
    m_facingDirection(0.0f, -1.0f),
    m_movementDirection(0.0f, 0.0f)
{
    m_texture = g_assetManager->loadTexture("Data/Sprites/tank_spritesheet.png");
}

Tank::~Tank()
{
}

void Tank::shootProjectile()
{
    // Create projectile.
    Projectile* projectile = new Projectile();
    projectile->setPosition(m_currentPosition + m_facingDirection * 0.4f);
    projectile->setDirection(m_facingDirection);
    GetWorld()->addObject(projectile);
}

void Tank::setMovementInput(sf::Vector2f movement)
{
    m_movementDirection = movement;
}

void Tank::onUpdate(float timeDelta)
{
    // Save previous player position.
    m_previousPosition = m_currentPosition;

    // Move player in direction.
    if(m_movementDirection != sf::Vector2f(0.0f, 0.0f))
    {
        // Update facing direction.
        m_facingDirection = m_movementDirection;

        // Update current position.
        m_currentPosition += m_movementDirection * timeDelta * 4.0f;
        m_movementDirection = sf::Vector2f(0.0f, 0.0f);
    }
}

void Tank::onDraw(float updateAlpha)
{
    // Draw tank.
    sf::Vector2f interpolatedPosition((1.0f - updateAlpha) * m_previousPosition + updateAlpha * m_currentPosition);

    sf::RectangleShape tankSprite;
    tankSprite.setSize(sf::Vector2f(1.0f, 1.0f));
    tankSprite.setOrigin(tankSprite.getSize() / 2.0f);
    tankSprite.setPosition(interpolatedPosition);
    tankSprite.setTexture(m_texture.get());

    if(m_facingDirection == sf::Vector2f(0.0f, -1.0f))
    {
        tankSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    }
    if(m_facingDirection == sf::Vector2f(1.0f, 0.0f))
    {
        tankSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
    }
    else if(m_facingDirection == sf::Vector2f(0.0f, 1.0f))
    {
        tankSprite.setTextureRect(sf::IntRect(0, 96, 32, 32));
    }
    else if(m_facingDirection == sf::Vector2f(-1.0f, 0.0f))
    {
        tankSprite.setTextureRect(sf::IntRect(0, 64, 32, 32));
    }

    g_render->draw(tankSprite);
}
