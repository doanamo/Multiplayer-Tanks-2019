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
        g_window->endRender();

        // Release unused resources.
        g_assetManager->releaseUnused();
    }

    // Shutdown globals.
    shutdownGlobals();
    return 0;
}
