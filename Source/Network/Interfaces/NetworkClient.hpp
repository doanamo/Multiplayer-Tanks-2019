#pragma once

#include "Precompiled.hpp"
#include "NetworkBase.hpp"

extern ConsoleVariable<std::string> cv_connect;

class NetworkClient : public NetworkBase
{
public:
    NetworkClient();
    ~NetworkClient();

    bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port);
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    NetworkMode getMode() const override;
    bool isConnected() const override;

private:
    // Heartbeat timer.
    float m_hearbeatTimer;
};
