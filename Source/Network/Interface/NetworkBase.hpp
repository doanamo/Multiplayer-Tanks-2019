#pragma once

#include "Precompiled.hpp"
#include "NetworkInterface.hpp"

extern ConsoleVariable<bool> cv_showNetwork;

class GameInstance;

class NetworkBase : public NetworkInterface
{
protected:
    NetworkBase();

public:
    // Virtual destructor.
    virtual ~NetworkBase();

    // Network methods.
    virtual bool initialize(GameInstance* gameInstance, const sf::IpAddress& address, unsigned short port);
    virtual void update(float timeDelta) override;
    virtual void tick(float timeDelta) override;
    virtual void draw() override;

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

protected:
    bool serializePacket(MemoryStream& stream, const PacketBase& packet);
    bool deserializePacket(MemoryStream& stream, std::unique_ptr<PacketBase>& packet);

protected:
    // Game instance.
    GameInstance* m_gameInstance;

    // Network socket.
    sf::UdpSocket m_udpSocket;
};
