#pragma once

#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Replication/ReplicationClient.hpp"

extern ConsoleVariable<std::string> cv_connect;

class NetworkClient : public NetworkBase
{
public:
    NetworkClient();
    ~NetworkClient();

    bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port);
    void update(float timeDelta) override;
    void preTick(float timeDelta) override;
    void postTick(float timeDelta) override;
    void draw() override;

    NetworkMode getMode() const override;
    bool isConnected() const override;

    ReplicationBase& getReplication() override final;

private:
    // Replication system.
    ReplicationClient m_replication;

    // Heartbeat timer.
    float m_hearbeatTimer;
};
