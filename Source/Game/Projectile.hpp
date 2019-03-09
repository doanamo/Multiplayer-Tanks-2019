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
    // Remaining projectile lifetime.
    float m_remainingLifetime;

    // Projectile sprite texture.
    std::shared_ptr<sf::Texture> m_texture;
};
