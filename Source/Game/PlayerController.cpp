#include "Precompiled.hpp"
#include "Game/PlayerController.hpp"
#include "Game/World.hpp"

PlayerController::PlayerController() :
    m_world(nullptr),
    m_object(),
    m_playerMovement(MovementDirection::None)
{
}

PlayerController::~PlayerController()
{
}

bool PlayerController::initialize(World* world)
{
    ASSERT(world != nullptr, "Cannot initialize player controller without correct world!");

    m_world = world;

    return true;
}

void PlayerController::control(Handle object)
{
    m_object = object;
}

void PlayerController::handleEvent(const sf::Event& event)
{
    // Get player tank object.
    Tank* playerTank = dynamic_cast<Tank*>(m_world->getObjectByName("Player1_Tank"));
    if(playerTank == nullptr) return;

    // Handle player tank input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Key::Space:
            playerTank->shootProjectile();
            break;

        case sf::Keyboard::Key::Up:
            m_playerMovement = MovementDirection::Up;
            break;

        case sf::Keyboard::Key::Down:
            m_playerMovement = MovementDirection::Down;
            break;

        case sf::Keyboard::Key::Left:
            m_playerMovement = MovementDirection::Left;
            break;

        case sf::Keyboard::Key::Right:
            m_playerMovement = MovementDirection::Right;
            break;
        }
    }
}

void PlayerController::tick(float timeDelta)
{
    // Get player tank object.
    Tank* playerTank = dynamic_cast<Tank*>(m_world->getObjectByName("Player1_Tank"));
    if(playerTank == nullptr) return;

    // Handle player tank movement.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if(m_playerMovement == MovementDirection::Up || m_playerMovement == MovementDirection::None)
        {
            playerTank->setMovementInput(sf::Vector2f(0.0f, -1.0f));
            m_playerMovement = MovementDirection::Up;
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if(m_playerMovement == MovementDirection::Down || m_playerMovement == MovementDirection::None)
        {
            playerTank->setMovementInput(sf::Vector2f(.0f, 1.0f));
            m_playerMovement = MovementDirection::Down;
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if(m_playerMovement == MovementDirection::Left || m_playerMovement == MovementDirection::None)
        {
            playerTank->setMovementInput(sf::Vector2f(-1.0f, 0.0f));
            m_playerMovement = MovementDirection::Left;
        }
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if(m_playerMovement == MovementDirection::Right || m_playerMovement == MovementDirection::None)
        {
            playerTank->setMovementInput(sf::Vector2f(1.0f, 0.0f));
            m_playerMovement = MovementDirection::Right;
        }
    }
    else
    {
        m_playerMovement = MovementDirection::None;
    }
}

bool PlayerController::onSerialize(MemoryBuffer& buffer) const
{
    return true;
}

bool PlayerController::onDeserialize(MemoryBuffer& buffer)
{
    // Temporary workaround since we cannot resolve handles yet.
    m_object.identifier = 1;
    m_object.version = 1;

    return true;
}
