#pragma once

#include "Precompiled.hpp"
#include "Objects/Tank.hpp"

class World;

class PlayerController : public Serializable
{
public:
    PlayerController();
    ~PlayerController();

    bool initialize(World* world);
    void control(ObjectHandle handle);
    void handleEvent(const sf::Event& event);
    void tick(float timeDelta);

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
};
