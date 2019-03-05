#include "Precompiled.hpp"
#include "Game/Tank.hpp"

Tank::Tank()
{
}

Tank::~Tank()
{
}

void Tank::onUpdate(float timeDelta)
{
    // Save previous player position.
    previousPosition = currentPosition;

    // Move player in direction.
    sf::Vector2f moveDirection;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveDirection.x -= 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveDirection.x += 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveDirection.y -= 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveDirection.y += 1.0f;

    currentPosition += moveDirection * timeDelta * 120.0f; // Todo: Normalize move direction.
}

void Tank::onDraw(float updateAlpha)
{
    // Draw the tank.
    sf::RectangleShape rectangleShape;
    rectangleShape.setFillColor(sf::Color::Green);
    rectangleShape.setSize(sf::Vector2f(32.0f, 32.0f));
    rectangleShape.setOrigin(sf::Vector2f(16.0f, 16.0f));
    rectangleShape.setPosition((1.0f - updateAlpha) * previousPosition + updateAlpha * currentPosition);

    g_render->draw(rectangleShape);
}
