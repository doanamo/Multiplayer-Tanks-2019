#pragma once

#include "Precompiled.hpp"

class Window
{
public:
    Window();
    ~Window();

    bool initialize(int width, int height, const char* name);
    void close();

    bool pollEvent(sf::Event& event);
    void beginRender();
    void endRender();

    bool isOpen() const;
    int getWidth() const;
    int getHeight() const;

    void setTitle(const sf::String title);
    const sf::String& getInitialTitle() const;

public:
    // Alias for render target.
    sf::RenderTarget& render;

private:
    // Render window instance.
    sf::RenderWindow m_window;

    // Internal window clock.
    sf::Clock m_clock;

    // Current window title.
    sf::String m_title;
};
