#pragma once

#include "Precompiled.hpp"

class WorldState
{
public:
    WorldState();

    bool initialize();
    void shutdown();

public:
    sf::RectangleShape playerShape;
    sf::Vector2f playerCurrentPosition;
    sf::Vector2f playerPreviousPosition;
};
