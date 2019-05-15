#pragma once

#include "Precompiled.hpp"
#include "Network/Network.hpp"

extern ConsoleVariable<std::string> cv_connect;

class Client : public Network
{
public:
    Client();
    ~Client();

    bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    bool isConnected() const override;
    bool isServer() const override;
    bool isClient() const override;

private:
    // Server info.
    sf::IpAddress m_serverAddress;
    unsigned short m_serverPort;

    // Connection socket.
    sf::TcpSocket m_tcpSocket;

    // Heartbeat timer.
    float m_hearbeatTimer;
};
