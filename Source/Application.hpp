#pragma once

#include "Precompiled.hpp"
#include "Game/Handle.hpp"
#include "Game/PlayerController.hpp"

class World;

class Application
{
public:
    Application();
    ~Application();

    bool initialize();

    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void draw(float updateAlpha);

public:
    // Game world.
    World* m_world;

    // Local player controller.
    PlayerController* m_playerController;
};
