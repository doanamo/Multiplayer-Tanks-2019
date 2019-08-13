#include "Precompiled.hpp"
#include "PlayerController.hpp"
#include "Game/World/World.hpp"

#define TEMP_DISABLE_CLIENT_INPUT true

PlayerController::PlayerController() :
    m_world(nullptr),
    m_object()
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

void PlayerController::control(ObjectHandle object)
{
    m_object = object;
}

void PlayerController::handleEvent(const sf::Event& event)
{
    // Get player tank object.
    Object* playerObject = m_world->getObjectByHandle(m_object);
    if(playerObject == nullptr)
        return;

    Tank* playerTank = playerObject->as<Tank>();
    if(playerTank == nullptr)
        return;

    // Handle player tank input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Key::Space:
            playerTank->shootProjectile();
            break;

        case sf::Keyboard::Key::Up:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Up];
                entry.inputTime = std::chrono::high_resolution_clock::now();
                entry.pressed = true;
            }
            break;

        case sf::Keyboard::Key::Down:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Down];
                entry.inputTime = std::chrono::high_resolution_clock::now();
                entry.pressed = true;
            }
            break;

        case sf::Keyboard::Key::Left:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Left];
                entry.inputTime = std::chrono::high_resolution_clock::now();
                entry.pressed = true;
            }
            break;

        case sf::Keyboard::Key::Right:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Right];
                entry.inputTime = std::chrono::high_resolution_clock::now();
                entry.pressed = true;
            }
            break;
        }
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Key::Up:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Up];
                entry.pressed = false;
            }
            break;

        case sf::Keyboard::Key::Down:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Down];
                entry.pressed = false;
            }
            break;

        case sf::Keyboard::Key::Left:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Left];
                entry.pressed = false;
            }   
            break;

        case sf::Keyboard::Key::Right:
            {
                MovementInputEntry& entry = m_movementInputs[MovementDirections::Right];
                entry.pressed = false;
            }
            break;
        }
    }
    else if(event.type == sf::Event::LostFocus)
    {
        for(auto& entry : m_movementInputs)
        {
            entry.pressed = false;
        }
    }
}

void PlayerController::tick(float timeDelta)
{
    // Get player tank object.
    Object* playerObject = m_world->getObjectByHandle(m_object);
    if(playerObject == nullptr)
        return;

    Tank* playerTank = playerObject->as<Tank>();
    if(playerTank == nullptr)
        return;

    // Get newest keyboard input.
    MovementDirections::Type movementDirection = MovementDirections::None;
    std::chrono::high_resolution_clock::time_point inputTime;

    for(int i = 0; i < MovementDirections::Max; ++i)
    {
        auto& movementInput = m_movementInputs[i];

        if(movementInput.pressed && movementInput.inputTime > inputTime)
        {
            movementDirection = (MovementDirections::Type)i;
            inputTime = movementInput.inputTime;
        }
    }

    // Handle player tank movement.
    if(movementDirection == MovementDirections::Up)
    {
        playerTank->setMovementInput(sf::Vector2f(0.0f, -1.0f));
    }
    else if(movementDirection == MovementDirections::Down)
    {
        playerTank->setMovementInput(sf::Vector2f(.0f, 1.0f));
    }
    else if(movementDirection == MovementDirections::Left)
    {
        playerTank->setMovementInput(sf::Vector2f(-1.0f, 0.0f));
    }
    else if(movementDirection == MovementDirections::Right)
    {
        playerTank->setMovementInput(sf::Vector2f(1.0f, 0.0f));
    }
}

bool PlayerController::onSerialize(MemoryStream& buffer) const
{
    return true;
}

bool PlayerController::onDeserialize(MemoryStream& buffer)
{
    // #todo: Regain control over own player object after game load.

    return true;
}
