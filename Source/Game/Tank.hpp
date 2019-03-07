#pragma once

#include "Precompiled.hpp"
#include "Game/Object.hpp"

class Tank : public Object
{
public:
    Tank();
    ~Tank();

    void SetMovementInput(sf::Vector2f movement);

    void onUpdate(float timeDelta) override;
    void onDraw(float updateAlpha) override;

private:
    sf::Vector2f m_currentPosition;
    sf::Vector2f m_previousPosition;
    sf::Vector2f m_facingDirection;
    sf::Vector2f m_movementDirection;
};
