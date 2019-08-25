#pragma once

#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

// Forward declarations.
class World;

// Player controller local class.
class PlayerControllerLocal : public PlayerControllerBase
{
    TYPE_DECLARE(PlayerControllerLocal, PlayerControllerBase);

public:
    PlayerControllerLocal();
    ~PlayerControllerLocal();

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
