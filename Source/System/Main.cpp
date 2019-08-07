#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"
#include "System/AssetManager.h"
#include "Application.hpp"

void exit(std::string arguments)
{
    if(g_window)
    {
        g_window->close();
    }
}

ConsoleFunction cf_exit("exit", &exit);

int main(int argc, char* argv[])
{
    // Initialize debug.
    initializeDebug();

    // Initialize globals.
    if(!initializeGlobals(argc, argv))
        return -1;

    // Prepare timer.
    sf::Clock timer;

    const float tickRate = 1.0f / 10.0f;
    float tickAccumulator = tickRate;

    // Run main loop.
    while(g_window->isOpen())
    {
        // Calculate time delta.
        float timeDelta = timer.restart().asSeconds();

        // Handle window events.
        sf::Event event;
        while(g_window->pollEvent(event))
        {
            g_application->handleEvent(event);
        }

        // Tick application.
        tickAccumulator += timeDelta;
        while(tickAccumulator >= tickRate)
        {
            g_application->tick(tickRate);
            tickAccumulator -= tickRate;
        }

        // Update application.
        g_application->update(timeDelta);

        // Render application.
        float timeAlpha = clamp(tickAccumulator / tickRate, 0.0f, 1.0f);

        g_window->beginRender();
        g_application->draw(timeAlpha);
        g_window->endRender();
    }

    // Shutdown globals.
    shutdownGlobals();
    return 0;
}
