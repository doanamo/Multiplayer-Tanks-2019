#pragma once

#include "Precompiled.hpp"
#include "NetworkInterface.hpp"
#include "Network/Connection/ConnectionSocket.hpp"

// Global declarations.
extern ConsoleVariable<bool> cv_showNetworkInfo;

// Forward declarations.
class GameInstance;
class ReplicationBase;

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
    virtual void preTick(float timeDelta) override;
    virtual void postTick(float timeDelta) override;
    virtual void draw() override;

    // Gets connection socket.
    ConnectionSocket& getSocket();

    // Gets replication system.
    virtual ReplicationBase& getReplication() = 0;

    // Checks whether network interface is hosting the game state.
    bool isHost() const override final;

protected:
    // Serializes and sends packet.
    bool sendPacket(ConnectionSocket& socket, PacketBase& packet, bool reliable, const sf::IpAddress* address = nullptr, const unsigned short* port = nullptr);

    // Receives and deserializes packet.
    bool receivePacket(ConnectionSocket& socket, std::unique_ptr<PacketBase>& packet, bool* reliable = nullptr, sf::IpAddress* address = nullptr, unsigned short* port = nullptr);

    // Deserializes packet from memory buffer.
    bool readPacket(MemoryStream& packetData, std::unique_ptr<PacketBase>& packet);

protected:
    // Connection socket.
    // Every client and server has at least one main socket.
    ConnectionSocket m_socket;

    // Game instance reference.
    GameInstance* m_gameInstance;
};
