#pragma once

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

private:

};
