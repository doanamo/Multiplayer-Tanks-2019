#pragma once

#include "Precompiled.hpp"

extern class RuntimeTypes* g_runtimeTypes;
extern class Window* g_window;
extern sf::RenderTarget* g_render;
extern class AssetManager* g_assetManager;
extern class Application* g_application;

bool initializeGlobals();
void shutdownGlobals();
