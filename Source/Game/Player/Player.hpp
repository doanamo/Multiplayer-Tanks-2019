#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "PlayerControllerBase.hpp"

// Player class.
class Player : public Serializable
{
public:
    // Type declarations.
    using PlayerHandle = Handle<Player>;

public:
    Player();
    ~Player();

    // Define move constructor.
    Player(Player&& player) = default;

    // Handles player input event.
    bool handleEvent(const sf::Event& event);

    // Ticks player.
    void tick(float timeDelta);

    // Sets player handle.
    void setPlayerHandle(PlayerHandle handle);

    // Sets player controller.
    void setPlayerController(std::unique_ptr<PlayerControllerBase>&& playerController);

    // Gets unique player handle.
    PlayerHandle getPlayerHandle() const;

    // Gets player controller.
    PlayerControllerBase* getPlayerController();

protected:
    // Replication of player entry info.
    virtual bool onSerialize(MemoryStream& stream) const override;
    virtual bool onDeserialize(MemoryStream& stream) override;

private:
    // Unique player handle.
    PlayerHandle m_playerHandle;

    // Player controller.
    std::unique_ptr<PlayerControllerBase> m_playerController;
};

// Declare public player handle type.
using PlayerHandle = Handle<Player>;
