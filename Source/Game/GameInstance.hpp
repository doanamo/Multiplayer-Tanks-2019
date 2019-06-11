#pragma once

#include "Precompiled.hpp"

class NetworkInterface;
class World;
class Level;
class PlayerController;

class GameInstance : public Serializable
{
public:
    GameInstance();
    ~GameInstance();

    bool initialize();
    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void tick(float timeDelta);
    void draw(float timeAlpha);

    uint64_t          getTickFrame() const;
    NetworkInterface* getNetwork();
    World*            getWorld();
    Level*            getLevel();
    PlayerController* getPlayerController();

protected:
    // Serialization methods.
    bool onSerialize(MemoryStream& buffer) const override;
    bool onDeserialize(MemoryStream& buffer) override;

private:
    // Current tick frame.
    uint64_t m_tickFrame;

    // Network interface.
    std::unique_ptr<NetworkInterface> m_network;
   
    // Objects in world.
    std::unique_ptr<World> m_world;

    // Game level.
    std::unique_ptr<Level> m_level;

    // Local player controller.
    std::unique_ptr<PlayerController> m_playerController;

    // Camera viewport
    sf::View m_viewport;

    bool m_isViewportCentered;
    bool m_isCameraAttachedToPlayer;
};
