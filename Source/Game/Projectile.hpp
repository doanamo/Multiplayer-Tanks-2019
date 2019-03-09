#pragma once

#include "Game/Object.hpp"

class Projectile : public Object
{
public:
    Projectile();
    ~Projectile();

    void setPosition(const sf::Vector2f& position);
    void setDirection(const sf::Vector2f& direction);

    void onUpdate(float timeDelta) override;
    void onDraw(float updateAlpha) override;

private:
    sf::Vector2f m_facingDirection;
    sf::Vector2f m_currentPosition;
    sf::Vector2f m_previousPosition;
    float m_remainingLifetime;

    std::shared_ptr<sf::Texture> m_texture;
};
