#pragma once

#include "Precompiled.hpp"
#include "Network/Interfaces/NetworkInterface.hpp"

// Forward declarations.
class World;
class Level;
class PlayerController;

// Game instance class.
class GameInstance : public Serializable
{
public:
    GameInstance();
    ~GameInstance();

    bool initialize(const NetworkParams& networkParams);
    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void tick(float timeDelta);
    void draw(float timeAlpha);

    // Gets current frame tick.
    uint64_t getTickFrame() const;

    // Gets world with objects.
    World& getWorld();

    // Gets level instance.
    Level& getLevel();

    // Gets player controller.
    PlayerController& getPlayerController();
    
    // Gets network interface.
    NetworkInterface& getNetwork();

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Tick counter.
    uint64_t m_tickCounter;

    // Game world.
    std::unique_ptr<World> m_world;

    // Game level.
    std::unique_ptr<Level> m_level;

    // Local player controller.
    std::unique_ptr<PlayerController> m_playerController;

    // Network interface.
    std::unique_ptr<NetworkInterface> m_network;

    // Camera viewport
    sf::View m_viewport;

    bool m_isViewportCentered;
    bool m_isCameraAttachedToPlayer;
};
