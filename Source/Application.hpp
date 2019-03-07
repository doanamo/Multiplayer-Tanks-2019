#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"

class World;

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void shutdown();

    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void draw(float updateAlpha);

public:
    // Game world.
    World* m_world;

    // Player handle.
    Handle m_player;

    enum class PlayerMovement
    {
        None,
        Up,
        Down,
        Left,
        Right,
    } m_playerMovement;
};
