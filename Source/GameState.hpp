#pragma once

#include "Precompiled.hpp"

class GameState
{
public:
    GameState();

    bool initialize();
    void shutdown();

public:
    sf::RectangleShape playerShape;
    sf::Vector2f playerCurrentPosition;
    sf::Vector2f playerPreviousPosition;
};
