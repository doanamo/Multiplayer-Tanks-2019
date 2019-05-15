#pragma once

#include "Game/Object.hpp"

class Projectile : public Object
{
    TYPE_DECLARE(Projectile, Object);

public:
    Projectile();
    ~Projectile();

    void setPosition(const sf::Vector2f& position);
    void setDirection(const sf::Vector2f& direction);

    void onTick(float timeDelta) override;
    void onDraw(float timeAlpha) override;

protected:
    // Serialization methods.
    virtual bool onSerialize(MemoryStream& buffer) const override;
    virtual bool onDeserialize(MemoryStream& buffer) override;

private:
    // Remaining projectile lifetime.
    float m_remainingLifetime;

    // Projectile sprite texture.
    std::shared_ptr<sf::Texture> m_texture;
};
