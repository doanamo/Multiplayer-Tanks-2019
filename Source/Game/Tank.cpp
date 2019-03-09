#include "Precompiled.hpp"
#include "Game/Tank.hpp"
#include "Game/Projectile.hpp"
#include "Game/World.hpp"

Tank::Tank() :
    m_currentPosition(0.0f, 0.0f),
    m_previousPosition(0.0f, 0.0f),
    m_facingDirection(0.0f, -1.0f),
    m_movementDirection(0.0f, 0.0f)
{
}

Tank::~Tank()
{
}

void Tank::shootProjectile()
{
    // Create projectile.
    Projectile* projectile = new Projectile();
    projectile->setPosition(m_currentPosition);
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
        m_currentPosition += m_movementDirection * timeDelta * 8.0f;
        m_movementDirection = sf::Vector2f(0.0f, 0.0f);
    }
}

void Tank::onDraw(float updateAlpha)
{
    // Draw the tank.
    sf::Vector2f interpolatedPosition((1.0f - updateAlpha) * m_previousPosition + updateAlpha * m_currentPosition);

    sf::RectangleShape tankShape;
    tankShape.setFillColor(sf::Color::Green);
    tankShape.setSize(sf::Vector2f(1.0f, 1.0f));
    tankShape.setOrigin(sf::Vector2f(0.5f, 0.5f));
    tankShape.setPosition(interpolatedPosition);
    g_render->draw(tankShape);

    sf::RectangleShape cannonShape;
    cannonShape.setFillColor(sf::Color::Yellow);
    cannonShape.setSize(sf::Vector2f(0.4f, 0.8f));
    cannonShape.setOrigin(sf::Vector2f(0.2f, 0.6f));
    cannonShape.setPosition(interpolatedPosition);
    
    if(m_facingDirection == sf::Vector2f(1.0f, 0.0f))
    {
        cannonShape.setRotation(90.0f);
    }
    else if(m_facingDirection == sf::Vector2f(0.0f, 1.0f))
    {
        cannonShape.setRotation(180.0f);
    }
    else if(m_facingDirection == sf::Vector2f(-1.0f, 0.0f))
    {
        cannonShape.setRotation(270.0f);
    }

    g_render->draw(cannonShape);
}
