#include "Precompiled.hpp"
#include "Window.h"
#include "AssetManager.h"

Window* g_window = nullptr;
AssetManager* g_assetManager = nullptr;

bool initialize();
void shutdown();

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

bool initialize()
{
    // Initialize window.
    g_window = new Window;

    if(!g_window->initialize())
    {
        shutdown();
        return false;
    }

    // Initialize asset manager.
    g_assetManager = new AssetManager;

    if(!g_assetManager->initialize())
    {
        shutdown();
        return false;
    }

    return true;
}

void shutdown()
{
    // Shutdown in reverse order.
    if(g_assetManager)
    {
        g_assetManager->shutdown();
        delete g_assetManager;
        g_assetManager = nullptr;
    }

    if(g_window)
    {
        g_window->shutdown();
        delete g_window;
        g_window = nullptr;
    }
}
