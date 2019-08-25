#include "Precompiled.hpp"
#include "PlayerControllerLocal.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"

PlayerControllerLocal::PlayerControllerLocal()
{
}

PlayerControllerLocal::~PlayerControllerLocal()
{
}

bool PlayerControllerLocal::handleEvent(const sf::Event& event)
{
    // Handle player tank input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Key::Space:
            this->pushPlayerCommand(PlayerCommand::Shoot);
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
        this->pushPlayerCommand(PlayerCommand::MoveUp);
    }
    else if(movementDirection == MovementDirections::Down)
    {
        this->pushPlayerCommand(PlayerCommand::MoveDown);
    }
    else if(movementDirection == MovementDirections::Left)
    {
        this->pushPlayerCommand(PlayerCommand::MoveLeft);
    }
    else if(movementDirection == MovementDirections::Right)
    {
        this->pushPlayerCommand(PlayerCommand::MoveRight);
    }
}

bool PlayerControllerLocal::onSerialize(MemoryStream& buffer) const
{
    // Serialize base class.
    if(!Super::onSerialize(buffer))
        return false;

    return true;
}

bool PlayerControllerLocal::onDeserialize(MemoryStream& buffer)
{
    // Deserialize base class.
    if(!Super::onDeserialize(buffer))
        return false;

    return true;
}
