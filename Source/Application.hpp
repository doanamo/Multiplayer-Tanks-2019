#pragma once

class WorldState;

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void shutdown();

    void handleEvent(sf::Event& event);
    void update(float timeDelta);
    void draw(float updateAlpha);

public:
    sf::View m_view;
    WorldState* m_gameState;
};
