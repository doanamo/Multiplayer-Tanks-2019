#pragma once

#include "Precompiled.hpp"

extern class Window* g_window;
extern class AssetManager* g_assetManager;
extern class Application* g_application;

bool initializeGlobals();
void shutdownGlobals();