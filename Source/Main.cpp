#include "Precompiled.hpp"
#include "Globals.hpp"
#include "Window.h"
#include "Application.hpp"

int main()
{
    // Initialize globals.
    if(!initializeGlobals())
        return -1;

    // Prepare update clock.
    const float updateRate = 1.0f / 20.0f;
    float updateAccumulator = updateRate;
    sf::Clock updateClock;

    // Run main loop.
    while(g_window->isOpen())
    {
        // Handle window events.
        sf::Event event;
        while(g_window->pollEvent(event))
        {
            g_application->handleEvent(event);
        }

        // Update application.
        updateAccumulator += updateClock.restart().asSeconds();

        while(updateAccumulator >= updateRate)
        {
            g_application->update(updateRate);
            updateAccumulator -= updateRate;
        }

        // Render application.
        float updateAlpha = updateAccumulator / updateRate;

        g_window->beginRender();
        g_application->draw(updateAlpha);
        ImGui::ShowDemoWindow();
        g_window->endRender();
    }

    // Shutdown globals.
    shutdownGlobals();
    return 0;
}
