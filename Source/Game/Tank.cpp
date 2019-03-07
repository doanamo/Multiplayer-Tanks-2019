#include "Precompiled.hpp"
#include "Game/Tank.hpp"

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

void Tank::SetMovementInput(sf::Vector2f movement)
{
    assert(((std::abs(movement.x) == 1.0f && movement.y == 0.0f) || (std::abs(movement.y) == 1.0f && movement.x == 0.0f)) && "Invalid non single directional movement input!");

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
        m_currentPosition += m_movementDirection * timeDelta * 120.0f;
        m_movementDirection = sf::Vector2f(0.0f, 0.0f);
    }
}

void Tank::onDraw(float updateAlpha)
{
    // Draw the tank.
    sf::Vector2f interpolatedPosition((1.0f - updateAlpha) * m_previousPosition + updateAlpha * m_currentPosition);

    sf::RectangleShape tankShape;
    tankShape.setFillColor(sf::Color::Green);
    tankShape.setSize(sf::Vector2f(32.0f, 32.0f));
    tankShape.setOrigin(sf::Vector2f(16.0f, 16.0f));
    tankShape.setPosition(interpolatedPosition);
    g_render->draw(tankShape);

    sf::RectangleShape cannonShape;
    cannonShape.setFillColor(sf::Color::Yellow);
    cannonShape.setSize(sf::Vector2f(14.0f, 22.0f));
    cannonShape.setOrigin(sf::Vector2f(7.0f, 20.0f));
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
