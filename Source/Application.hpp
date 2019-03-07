#pragma once

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
};
