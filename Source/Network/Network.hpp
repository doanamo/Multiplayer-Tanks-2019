#pragma once

#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

extern ConsoleVariable<bool> cv_showNetwork;
class GameInstance;

class Network
{
protected:
    Network();

public:
    virtual ~Network();

    // Loop methods.
    virtual bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port);
    virtual void update(float timeDelta);
    virtual void tick(float timeDelta);
    virtual void draw();

    // Sending and receiving packets.
    bool sendTcpPacket(const PacketBase& packet, sf::TcpSocket& socket);
    bool receiveTcpPacket(std::unique_ptr<PacketBase>& packet, sf::TcpSocket& socket);

    bool sendUdpPacket(const PacketBase& packet, const sf::IpAddress& address, unsigned short port);
    bool receiveUdpPacket(std::unique_ptr<PacketBase>& packet, sf::IpAddress& address, unsigned short& port);

    // Sending and receiving data buffers.
    bool sendTcpData(const MemoryStream& buffer, sf::TcpSocket& socket);
    bool receiveTcpData(MemoryStream& buffer, sf::TcpSocket& socket);

    bool sendUdpData(const MemoryStream& buffer, const sf::IpAddress& address, unsigned short port);
    bool receiveUdpData(MemoryStream& buffer, sf::IpAddress& address, unsigned short& port);

    // Network info.
    virtual bool isConnected() const;
    virtual bool isServer() const;
    virtual bool isClient() const;

protected:
    // Game instance.
    GameInstance* m_gameInstance;

    // Network socket.
    sf::UdpSocket m_udpSocket;
};
