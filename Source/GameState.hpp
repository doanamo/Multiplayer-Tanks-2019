#pragma once

#include "Precompiled.hpp"

class GameState
{
public:
    GameState();

public:
    sf::RectangleShape playerShape;
    sf::Vector2f playerCurrentPosition;
    sf::Vector2f playerPreviousPosition;
};
