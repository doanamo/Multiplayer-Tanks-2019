#pragma once

#include "Precompiled.hpp"
#include "NetworkInterface.hpp"
#include "Network/Connection/ConnectionSocket.hpp"

// Global declarations.
extern ConsoleVariable<bool> cv_showNetwork;

// Forward declarations.
class GameInstance;

// Network base class.
class NetworkBase : public NetworkInterface
{
protected:
    NetworkBase();

public:
    // Virtual destructor.
    virtual ~NetworkBase();

    // Network methods.
    bool initialize(GameInstance* gameInstance);
    virtual void update(float timeDelta) override;
    virtual void tick(float timeDelta) override;
    virtual void draw() override;

protected:
    // Serializes and sends packet.
    bool sendPacket(ConnectionSocket& socket, PacketBase& packet, bool reliable, const sf::IpAddress* address = nullptr, const unsigned short* port = nullptr);

    // Receives and deserializes packet.
    bool receivePacket(ConnectionSocket& socket, std::unique_ptr<PacketBase>& packet, bool* reliable = nullptr, sf::IpAddress* address = nullptr, unsigned short* port = nullptr);

protected:
    // Connection socket.
    // Every client and server has at least one main socket.
    ConnectionSocket m_socket;

    // Game instance.
    GameInstance* m_gameInstance;
};
