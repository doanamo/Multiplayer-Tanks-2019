#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "System/AssetManager.h"
#include "Application.hpp"

int main()
{
    // Initialize debug.
    initializeDebug();

    // Initialize globals.
    if(!initializeGlobals())
        return -1;

    // Prepare timer.
    sf::Clock timer;

    const float tickRate = 1.0f / 20.0f;
    float tickAccumulator = tickRate;

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
        float timeDelta = timer.restart().asSeconds();

        g_application->update(timeDelta);

        // Tick application.
        tickAccumulator += timeDelta;

        while(tickAccumulator >= tickRate)
        {
            g_application->tick(tickRate);
            tickAccumulator -= tickRate;
        }

        // Render application.
        float timeAlpha = tickAccumulator / tickRate;

        g_window->beginRender();
        g_application->draw(timeAlpha);
        g_window->endRender();
    }

    // Shutdown globals.
    shutdownGlobals();
    return 0;
}
