#pragma once

#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

// Forward declarations.
class World;

// Player controller human class.
class PlayerControllerHuman : public PlayerControllerBase
{
    TYPE_DECLARE(PlayerControllerHuman, PlayerControllerBase);

public:
    PlayerControllerHuman();
    ~PlayerControllerHuman();

    // Handles player input event.
    bool handleEvent(const sf::Event& event) override;

    // Processes player tick.
    void tick(float timeDelta) override;

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Movement direction handling.
    struct MovementDirections
    {
        enum Type : uint8_t
        {
            Up,
            Down,
            Left,
            Right,
            Max,

            None,
        };
    };

    struct MovementInputEntry
    {
        std::chrono::high_resolution_clock::time_point inputTime;
        bool pressed = false;
    };

    MovementInputEntry m_movementInputs[MovementDirections::Max];
};
