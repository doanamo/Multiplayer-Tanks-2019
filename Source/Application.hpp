#pragma once

#include "Precompiled.hpp"

class Network;
class GameInstance;

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void tick(float timeDelta);
    void draw(float timeAlpha);

    bool saveSnapshot();
    bool loadSnaphot();

public:
    // Game instance.
    GameInstance* m_gameInstance;

private:
    // Game viewport TODO: move viewport to GameInstance
    sf::View viewport;

    bool isViewportCentered;
    bool isCameraAttachedToPlayer;
};
