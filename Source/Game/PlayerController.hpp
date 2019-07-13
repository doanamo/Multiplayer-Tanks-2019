#pragma once

#include "Precompiled.hpp"
#include "Objects/Tank.hpp"

class World;

class PlayerController : public Serializable
{
public:
    PlayerController();
    ~PlayerController();

    bool initialize(World* world);
    void control(ObjectHandle handle);
    void handleEvent(const sf::Event& event);
    void tick(float timeDelta);

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Controlled object.
    World* m_world;
    ObjectHandle m_object;

    // Movement direction.
    enum class MovementDirection
    {
        None,
        Up,
        Down,
        Left,
        Right,
    };

    MovementDirection m_playerMovement;
};
