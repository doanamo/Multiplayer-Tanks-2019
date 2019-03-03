#include "Precompiled.hpp"
#include "Globals.hpp"
#include "Window.h"
#include "AssetManager.h"
#include "Application.hpp"
#include "GameState.hpp"

Window* g_window = nullptr;
AssetManager* g_assetManager = nullptr;
Application* g_application = nullptr;
GameState* g_gameState = nullptr;

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

    // Create game state instance.
    g_gameState = new GameState;

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

    if(g_gameState)
    {
        delete g_gameState;
        g_gameState = nullptr;
    }

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
