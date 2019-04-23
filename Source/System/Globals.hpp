#pragma once

extern class Logger* g_logger;
extern class CommandLine* g_commandLine;
extern class Window* g_window;
extern class sf::RenderTarget* g_render;
extern class Console* g_console;
extern class AssetManager* g_assetManager;
extern class Application* g_application;

bool initializeGlobals(int argc, char* argv[]);
void shutdownGlobals();
