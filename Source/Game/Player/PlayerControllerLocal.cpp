#include "Precompiled.hpp"
#include "PlayerControllerLocal.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"

PlayerControllerLocal::PlayerControllerLocal() :
    m_world(nullptr),
    m_object(),
    m_initialized(false)
{
}

PlayerControllerLocal::~PlayerControllerLocal()
{
}

bool PlayerControllerLocal::initialize(World* world)
{
    // Save world reference.
    ASSERT(world != nullptr, "Cannot initialize player controller without correct world!");
    m_world = world;

    // Initialization state.
    m_initialized = true;
    return true;
}

void PlayerControllerLocal::control(ObjectHandle object)
{
    ASSERT(m_initialized);
    m_object = object;
}

bool PlayerControllerLocal::handleEvent(const sf::Event& event)
{
    ASSERT(m_initialized);

    // Get player tank object.
    Object* playerObject = m_world->getObjectByHandle(m_object);
    if(playerObject == nullptr)
        return true;

    Tank* playerTank = playerObject->as<Tank>();
    if(playerTank == nullptr)
        return true;

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

    return true;
}

void PlayerControllerLocal::tick(float timeDelta)
{
    ASSERT(m_initialized);

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

bool PlayerControllerLocal::onSerialize(MemoryStream& buffer) const
{
    ASSERT(m_initialized);

    return true;
}

bool PlayerControllerLocal::onDeserialize(MemoryStream& buffer)
{
    // #todo: Regain control over own player object after game load.

    return true;
}
