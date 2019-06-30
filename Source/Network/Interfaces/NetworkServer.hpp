#pragma once

#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Connection/ConnectionSocket.hpp"

// Forward declarations.
class ConnectionBackend;

// Network server class.
class NetworkServer : public NetworkBase
{
public:
    NetworkServer();
    ~NetworkServer();

    bool initialize(GameInstance* gameInstance, unsigned short port);
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    NetworkMode getMode() const override;
    bool isConnected() const override;

private:
    // Default connection socket.
    ConnectionSocket m_socket;

    // List of clients.
    struct ClientEntry
    {
        std::unique_ptr<ConnectionSocket> socket;
    };

    std::vector<ClientEntry> m_clients;
};
