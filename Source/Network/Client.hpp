#pragma once

#include "Precompiled.hpp"
#include "Network/Network.hpp"

extern ConsoleVariable<std::string> cv_connect;

class Client : public Network
{
public:
    Client();
    ~Client();

    bool initialize() override;
    void onUpdate(float timeDelta) override;
    void onTick(float timeDelta) override;

    bool isConnected() const override;
    bool isServer() const override;
    bool isClient() const override;
    int getClientIndex() const override;

private:
    // Server info.
    sf::IpAddress m_serverAddress;
    unsigned short m_serverPort;

    // Heartbeat timer.
    float m_hearbeatTimer;
};
