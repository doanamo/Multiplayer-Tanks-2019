#pragma once

#include "Precompiled.hpp"

class Window
{
public:
    Window();
    ~Window();

public:
    bool initialize();
    void shutdown();

    bool pollEvent(sf::Event& event);
    void beginRender();
    void endRender();

    bool isOpen() const;
    int getWidth() const;
    int getHeight() const;

public:
    sf::RenderTarget& render;

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;

    const char* m_name = "Tanks Project";
    const unsigned int m_widht = 1024;
    const unsigned int m_height = 576;
};
