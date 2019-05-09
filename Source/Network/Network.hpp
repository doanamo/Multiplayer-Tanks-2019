#pragma once

#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

extern ConsoleVariable<bool> cv_showNetwork;

class Network
{
protected:
    Network();

public:
    virtual ~Network();

    // Loop methods.
    virtual bool initialize(const sf::IpAddress& address, unsigned short port);
    virtual void update(float timeDelta);
    virtual void tick(float timeDelta);
    virtual void draw();

    // Sending and receiving packets.
    bool sendPacket(PacketBase& packet, const sf::IpAddress& address, unsigned short port);
    bool receivePacket(std::unique_ptr<PacketBase>& packet, sf::IpAddress& address, unsigned short& port);

    // Sending and receiving data buffers.
    bool sendData(const MemoryBuffer& buffer, const sf::IpAddress& address, unsigned short port);
    bool receiveData(MemoryBuffer& buffer, sf::IpAddress& address, unsigned short& port);

    // Network info.
    virtual bool isConnected() const;
    virtual bool isServer() const;
    virtual bool isClient() const;

protected:
    // Network socket.
    sf::UdpSocket m_udpSocket;
};
