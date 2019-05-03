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

    // Network interface.
    Network* m_network;

private:
    // Camera viewport
    sf::View m_viewport;

    bool m_isViewportCentered;
    bool m_isCameraAttachedToPlayer;
};
