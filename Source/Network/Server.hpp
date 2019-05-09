#pragma once

#include "Precompiled.hpp"
#include "Network/Network.hpp"

class Server : public Network
{
public:
    Server();
    ~Server();

    bool initialize(const sf::IpAddress& address, unsigned short port) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    bool isConnected() const override;
    bool isServer() const override;

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
