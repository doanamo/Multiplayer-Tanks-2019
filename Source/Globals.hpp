#pragma once

#include "Precompiled.hpp"

extern class Window* g_window;
extern class AssetManager* g_assetManager;

bool initializeGlobals();
void shutdownGlobals();
