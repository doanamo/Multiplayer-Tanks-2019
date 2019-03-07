#include "Precompiled.hpp"
#include "Game/Projectile.hpp"

Projectile::Projectile() :
    m_currentPosition(0.0, 0.0f),
    m_previousPosition(0.0f, 0.0f),
    m_facingDirection(0.0f, -1.0f),
    m_remainingLifetime(2.0f)
{
}

Projectile::~Projectile()
{
}

void Projectile::setPosition(const sf::Vector2f& position)
{
    m_previousPosition = m_currentPosition = position;
}

void Projectile::setDirection(const sf::Vector2f& direction)
{
    m_facingDirection = direction;
}

void Projectile::onUpdate(float timeDelta)
{
    // Save previous position.
    m_previousPosition = m_currentPosition;

    // Move in facing direction.
    m_currentPosition = m_currentPosition + m_facingDirection * 300.0f * timeDelta;

    // Destroy projectile at the end of its lifetime.
    m_remainingLifetime = std::max(0.0f, m_remainingLifetime - timeDelta);

    if(m_remainingLifetime == 0.0f)
    {
        destroy();
    }
}

void Projectile::onDraw(float updateAlpha)
{
    // Draw bullet shape.
    sf::Vector2f interpolatedPosition((1.0f - updateAlpha) * m_previousPosition + updateAlpha * m_currentPosition);

    sf::RectangleShape projectileShape;
    projectileShape.setFillColor(sf::Color::Red);
    projectileShape.setSize(sf::Vector2f(8.0f, 8.0f));
    projectileShape.setOrigin(sf::Vector2f(4.0, 4.0));
    projectileShape.setPosition(interpolatedPosition);
    g_render->draw(projectileShape);
}
