#pragma once

#include "Precompiled.hpp"

class World;
class Level;
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
    Level* getLevel();
    PlayerController* getPlayerController();

private:
    // Game world.
    World* m_world;

    // Game level.
    Level* m_level;

    // Local player controller.
    PlayerController* m_playerController;

    // Local player camera.
    // PlayerCamera* m_playerCamera;
};
