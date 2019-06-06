#pragma once

#include "Precompiled.hpp"
#include "NetworkBase.hpp"

class NetworkServer : public NetworkBase
{
public:
    NetworkServer();
    ~NetworkServer();

    bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    NetworkMode getMode() const override;
    bool isConnected() const override;

private:
    // List of clients.
    struct ClientEntry
    {
        std::unique_ptr<sf::TcpSocket> socket;
    };

    std::vector<ClientEntry> m_clients;

    // Incoming connection listener.
    sf::TcpListener m_tcpListener;
};
