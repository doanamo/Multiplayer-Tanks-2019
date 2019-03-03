#include "Precompiled.hpp"
#include "Window.h"

Window* g_window = nullptr;

void shutdown()
{
    // Shutdown in reverse order.
    g_window->shutdown();
    delete g_window;
}

bool initialize()
{
    // Initialize window.
    g_window = new Window;

    if(!g_window->initialize())
    {
        shutdown();
        return false;
    }

    return true;
}

int main()
{
    // Initialize everything.
    if(!initialize())
        return -1;

    // Temp test shape.
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Run main loop.
    while(g_window->isOpen())
    {
        sf::Event event;
        while(g_window->pollEvent(event))
        {
        }

        g_window->beginRender();
        ImGui::ShowDemoWindow();
        g_window->render.draw(shape);
        g_window->endRender();
    }

    // Shutdown systems
    shutdown();
    return 0;
}
