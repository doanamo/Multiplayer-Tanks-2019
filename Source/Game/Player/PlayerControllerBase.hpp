#pragma once

#include "Precompiled.hpp"
#include "PlayerCommand.hpp"
#include "Game/World/Object.hpp"

#define ENABLE_PLAYER_CONTROLLER_LOG_TRACE false

// Base version of this class only stores player commands without any logic.
class PlayerControllerBase : public Serializable
{
    TYPE_DECLARE(PlayerControllerBase);

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

    // Returns whether controller should be serializable.
    virtual bool isSerializable() const;

protected:
    // Pushes player command.
    void pushPlayerCommand(PlayerCommand playerCommand);

    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Controlled object handle.
    ObjectHandle m_controlledObject;

    // Player command queue.
    std::queue<PlayerCommand> m_playerCommands;
};
