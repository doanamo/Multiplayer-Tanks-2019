#pragma once

#include "Precompiled.hpp"
#include "Game/Tank.hpp"

class World;

class PlayerController
{
public:
    PlayerController();
    ~PlayerController();

    bool initialize(World* world);
    void control(Handle handle);
    void handleEvent(const sf::Event& event);
    void update(float elpasedTime);

private:
    // Controlled object.
    World* m_world;
    Handle m_object;

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
