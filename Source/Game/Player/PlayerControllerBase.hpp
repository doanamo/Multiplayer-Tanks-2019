#pragma once

#include "Precompiled.hpp"
#include "PlayerCommand.hpp"
#include "Game/World/Object.hpp"

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

    // Pops player command.
    PlayerCommand popPlayerCommand();

    // Sets controlled object handle.
    void setControlledObject(const ObjectHandle& objectHandle);

    // Gets controlled object handle.
    const ObjectHandle& getControlledObject() const;

protected:
    // Pushes player command.
    void pushPlayerCommand(PlayerCommand playerCommand);

private:
    // Controlled object handle.
    ObjectHandle m_controlledObject;

    // Player command queue.
    std::queue<PlayerCommand> m_playerCommands;
};
