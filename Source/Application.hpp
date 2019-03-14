#pragma once

#include "Precompiled.hpp"

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
    // Game instance.
    GameInstance* m_gameInstance;
};
