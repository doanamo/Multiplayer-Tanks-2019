#include "Precompiled.hpp"
#include "Game/Projectile.hpp"
#include "System/AssetManager.h"

Projectile::Projectile() :
    m_currentPosition(0.0, 0.0f),
    m_previousPosition(0.0f, 0.0f),
    m_facingDirection(0.0f, -1.0f),
    m_remainingLifetime(2.0f)
{
    m_texture = g_assetManager->loadTexture("Data/Sprites/projectile_base.png");
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
    m_currentPosition = m_currentPosition + m_facingDirection * 6.0f * timeDelta;

    // Destroy projectile at the end of its lifetime.
    m_remainingLifetime = std::max(0.0f, m_remainingLifetime - timeDelta);

    if(m_remainingLifetime == 0.0f)
    {
        destroy();
    }
}

void Projectile::onDraw(float updateAlpha)
{
    // Draw projectile.
    sf::Vector2f interpolatedPosition((1.0f - updateAlpha) * m_previousPosition + updateAlpha * m_currentPosition);

    sf::RectangleShape projectileSprite;
    projectileSprite.setSize(sf::Vector2f(0.3f, 0.3f));
    projectileSprite.setOrigin(projectileSprite.getSize() / 2.0f);
    projectileSprite.setPosition(interpolatedPosition);
    projectileSprite.setTexture(m_texture.get());

    if(m_facingDirection == sf::Vector2f(0.0f, -1.0f))
    {
        projectileSprite.setRotation(0.0f);
    }
    if(m_facingDirection == sf::Vector2f(1.0f, 0.0f))
    {
        projectileSprite.setRotation(90.0f);
    }
    else if(m_facingDirection == sf::Vector2f(0.0f, 1.0f))
    {
        projectileSprite.setRotation(180.0f);
    }
    else if(m_facingDirection == sf::Vector2f(-1.0f, 0.0f))
    {
        projectileSprite.setRotation(270.0f);
    }

    projectileSprite.setPosition(projectileSprite.getPosition() + sf::Vector2f(0.0f, -0.05f));

    g_render->draw(projectileSprite);
}
