#pragma once

#include "Precompiled.hpp"
#include "Game/Object.hpp"

class Tank : public Object
{
public:
    Tank();
    ~Tank();

    void onUpdate(float timeDelta) override;
    void onDraw(float updateAlpha) override;

public:
    sf::Vector2f currentPosition;
    sf::Vector2f previousPosition;
};
