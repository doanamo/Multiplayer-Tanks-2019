#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "Types/RuntimeTypes.hpp"
#include "System/Window.h"
#include "System/AssetManager.h"
#include "Application.hpp"

RuntimeTypes* g_runtimeTypes = nullptr;
Window* g_window = nullptr;
sf::RenderTarget* g_render = nullptr;
AssetManager* g_assetManager = nullptr;
Application* g_application = nullptr;

bool initializeGlobals()
{
    // Initialize runtime types.
    g_runtimeTypes = new RuntimeTypes;

    if(!g_runtimeTypes->initialize())
    {
        shutdownGlobals();
        return false;
    }

    // Initialize window.
    g_window = new Window;

    if(!g_window->initialize())
    {
        shutdownGlobals();
        return false;
    }

    // Create render target pointer without exposing window.
    g_render = &g_window->render;

    // Initialize asset manager.
    g_assetManager = new AssetManager;

    if(!g_assetManager->initialize())
    {
        shutdownGlobals();
        return false;
    }

    // Initialize application.
    g_application = new Application;

    if(!g_application->initialize())
    {
        shutdownGlobals();
        return false;
    }

    return true;
}

void shutdownGlobals()
{
    // Shutdown in reverse order.
    if(g_application)
    {
        delete g_application;
        g_application = nullptr;
    }

    if(g_assetManager)
    {
        delete g_assetManager;
        g_assetManager = nullptr;
    }

    if(g_window)
    {
        delete g_window;
        g_window = nullptr;
        g_render = nullptr;
    }

    if(g_runtimeTypes)
    {
        delete g_runtimeTypes;
        g_runtimeTypes = nullptr;
    }
}
