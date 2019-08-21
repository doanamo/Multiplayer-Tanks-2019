#pragma once

#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"
#include "Game/World/Object.hpp"

// Forward declarations.
class World;

// Player controller local class.
class PlayerControllerLocal : public PlayerControllerBase
{
public:
    PlayerControllerLocal();
    ~PlayerControllerLocal();

    // Initializes player controller.
    bool initialize(World* world);
    
    // Controls object with specified handle.
    void control(ObjectHandle handle);

    // Handles player input event.
    bool handleEvent(const sf::Event& event) override;

    // Processes player tick.
    void tick(float timeDelta) override;

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Controlled object.
    World* m_world;
    ObjectHandle m_object;

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

    // Initialization state.
    bool m_initialized;
};
