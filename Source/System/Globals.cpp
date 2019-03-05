#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "System/AssetManager.h"
#include "Application.hpp"

Window* g_window = nullptr;
sf::RenderTarget* g_render = nullptr;
AssetManager* g_assetManager = nullptr;
Application* g_application = nullptr;

bool initializeGlobals()
{
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
        g_application->shutdown();
        delete g_application;
        g_application = nullptr;
    }

    if(g_assetManager)
    {
        g_assetManager->shutdown();
        delete g_assetManager;
        g_assetManager = nullptr;
    }

    if(g_render)
    {
        // Managed by window instance.
        g_render = nullptr;
    }

    if(g_window)
    {
        g_window->shutdown();
        delete g_window;
        g_window = nullptr;
    }
}
