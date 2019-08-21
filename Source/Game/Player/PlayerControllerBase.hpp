#pragma once

#include "Precompiled.hpp"

// Player controller base class.
class PlayerControllerBase : public Serializable
{
protected:
    PlayerControllerBase();

public:
    virtual ~PlayerControllerBase();

    // Handle player input event.
    virtual bool handleEvent(const sf::Event& event);

    // Ticks player controller.
    virtual void tick(float timeDelta);

private:
    // Accumulated player commands.
    // #todo
};
