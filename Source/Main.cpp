#include "Precompiled.hpp"
#include "Globals.hpp"
#include "Window.h"

int main()
{
    // Initialize globals.
    if(!initializeGlobals())
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

    // Shutdown globals.
    shutdownGlobals();
    return 0;
}
