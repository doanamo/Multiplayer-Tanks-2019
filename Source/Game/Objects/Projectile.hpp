#pragma once

#include "Game/World/Object.hpp"
#include "Network/Replication/Replicable.hpp"

class Projectile : public Replicable
{
    TYPE_DECLARE(Projectile, Replicable);

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

    // Replication methods.
    virtual bool serializeInitialReplication(MemoryStream& stream) override;
    virtual bool deserializeInitialReplication(MemoryStream& stream) override;

private:
    // Remaining projectile lifetime.
    float m_remainingLifetime;

    // Projectile sprite texture.
    std::shared_ptr<sf::Texture> m_texture;
};
