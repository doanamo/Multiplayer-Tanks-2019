#pragma once

#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Replication/ReplicationClient.hpp"
#include "Game/Player/Player.hpp"

// Forward declarations.
class PlayerControllerRemote;

// Console variable used by launcher to connect immediately on game launch.
extern ConsoleVariable<std::string> cv_connect;

// Network client class.
class NetworkClient : public NetworkBase
{
public:
    NetworkClient();
    ~NetworkClient();

    // Initializes client network interface.
    bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port);

    // Updates client network interface.
    void update(float timeDelta) override;

    // Processing done before game tick.
    void preTick(float timeDelta) override;

    // Processing done after game tick.
    void postTick(float timeDelta) override;

    // Draws debug interface.
    void draw() override;

    // Gets network interface mode.
    NetworkMode getMode() const override;

    // Checks whether interface is connected.
    bool isConnected() const override;

    // Gets replication system.
    ReplicationBase& getReplication() override final;

    // Gets player's handle.
    const PlayerHandle& getPlayerHandle() const;

private:
    // Gets client's remote controller.
    PlayerControllerRemote* getRemoteController();

private:
    // Replication system.
    ReplicationClient m_replication;

    // Player handle.
    PlayerHandle m_playerHandle;

    // Heartbeat timer.
    float m_hearbeatTimer;
};
