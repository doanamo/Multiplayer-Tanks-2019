#include "Precompiled.hpp"
#include "Globals.hpp"
#include "Window.h"
#include "AssetManager.h"

Window* g_window = nullptr;
AssetManager* g_assetManager = nullptr;

bool initializeGlobals()
{
    // Initialize window.
    g_window = new Window;

    if(!g_window->initialize())
    {
        shutdownGlobals();
        return false;
    }

    // Initialize asset manager.
    g_assetManager = new AssetManager;

    if(!g_assetManager->initialize())
    {
        shutdownGlobals();
        return false;
    }

    return true;
}

void shutdownGlobals()
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
