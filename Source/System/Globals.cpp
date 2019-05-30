#include "Precompiled.hpp"
#include "System/Globals.hpp"
#include "System/CommandLine.hpp"
#include "Types/RuntimeTypes.hpp"
#include "System/Window.hpp"
#include "System/Console.hpp"
#include "System/AssetManager.h"
#include "Application.hpp"

std::unique_ptr<Logger> g_logger;
std::unique_ptr<CommandLine> g_commandLine;
std::unique_ptr<RuntimeTypes> g_runtimeTypes;
std::unique_ptr<Window> g_window;
std::unique_ptr<Console> g_console;
std::unique_ptr<AssetManager> g_assetManager;
std::unique_ptr<Application> g_application;
sf::RenderTarget* g_renderTarget = nullptr;

namespace
{
    // Globals cleanup helper.
    struct ScopedGlobalsCleanup
    {
        ~ScopedGlobalsCleanup()
        {
            if(cleanup)
            {
                shutdownGlobals();
            }
        }

        bool cleanup = true;
    };
}

bool initializeGlobals(int argc, char* argv[])
{
    // Create scoped globals cleaner in case of failure.
    ScopedGlobalsCleanup scopedGuard;

    // Initialize logger.
    g_logger = std::make_unique<Logger>();

    if(!g_logger->initialize())
    {
        LOG_ERROR("Could not initialize logger!");
        return false;
    }

    // Initialize command line.
    g_commandLine = std::make_unique<CommandLine>();

    if(!g_commandLine->initialize(argc, argv))
    {
        LOG_ERROR("Could not initialize command line!");
        return false;
    }

    // Initialize runtime types.
    g_runtimeTypes = std::make_unique<RuntimeTypes>();

    if(!g_runtimeTypes->initialize())
    {
        LOG_ERROR("Could not initialize runtime types!");
        return false;
    }

    // Initialize window.
    g_window = std::make_unique<Window>();

    if(!g_window->initialize(1024, 576, "Tanks Project"))
    {
        LOG_ERROR("Could not initialize window!");
        return false;
    }

    // Create render target pointer without exposing window.
    g_renderTarget = &g_window->render;

    // Initialize console window.
    g_console = std::make_unique<Console>();

    if(!g_console->initialize())
    {
        LOG_ERROR("Could not initialize console!");
        return false;
    }

    // Parse command line console variables.
    g_commandLine->parseConsoleVariables();

    // Initialize asset manager.
    g_assetManager = std::make_unique<AssetManager>();

    if(!g_assetManager->initialize())
    {
        LOG_ERROR("Could not initialize asset manager!");
        return false;
    }

    // Initialize application.
    g_application = std::make_unique<Application>();

    if(!g_application->initialize())
    {
        LOG_ERROR("Could not initialize application!");
        return false;
    }

    // Success!
    scopedGuard.cleanup = false;
    return true;
}

void shutdownGlobals()
{
    // Shutdown in reverse order.
    g_renderTarget = nullptr;
    g_application = nullptr;
    g_assetManager = nullptr;
    g_console = nullptr;
    g_window = nullptr;
    g_runtimeTypes = nullptr;
    g_commandLine = nullptr;
    g_logger = nullptr;
}
