#pragma once

class Logger;
class CommandLine;
class RuntimeTypes;
class Window;
class Console;
class AssetManager;
class Application;
class sf::RenderTarget;

extern std::unique_ptr<Logger> g_logger;
extern std::unique_ptr<CommandLine> g_commandLine;
extern std::unique_ptr<RuntimeTypes> g_runtimeTypes;
extern std::unique_ptr<Window> g_window;
extern std::unique_ptr<Console> g_console;
extern std::unique_ptr<AssetManager> g_assetManager;
extern std::unique_ptr<Application> g_application;
extern sf::RenderTarget* g_renderTarget;

bool initializeGlobals(int argc, char* argv[]);
void shutdownGlobals();
