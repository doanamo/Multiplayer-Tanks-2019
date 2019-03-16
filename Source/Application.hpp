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
    void draw(float updateAlpha);

    bool saveSnapshot();
    bool loadSnaphot();

public:
    // Network interface.
    Network* m_network;

    // Game instance.
    GameInstance* m_gameInstance;
};
