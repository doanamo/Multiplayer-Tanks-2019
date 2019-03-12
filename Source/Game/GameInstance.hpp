#pragma once

#include "Precompiled.hpp"

class World;
class PlayerController;

class GameInstance
{
public:
    GameInstance();
    ~GameInstance();

    bool initialize();
    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void draw(float updateAlpha);

    World* getWorld();
    PlayerController* getPlayerController();

private:
    // Game world.
    World* m_world;

    // Local player controller.
    PlayerController* m_playerController;

    // Local player camera.
    // PlayerCamera* m_playerCamera;
};
