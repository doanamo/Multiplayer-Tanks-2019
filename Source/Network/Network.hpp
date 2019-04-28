#pragma once

#include "Precompiled.hpp"
#include "Network/PacketBase.hpp"

bool ParseStringToPort(std::string text, unsigned short& port);

class Network
{
protected:
    Network();

public:
    virtual ~Network();

    virtual bool initialize() = 0;
    virtual void onUpdate(float timeDelta) = 0;
    virtual void onTick(float timeDelta) = 0;

    virtual bool isConnected() const = 0;
    virtual bool isServer() const = 0;
    virtual bool isClient() const = 0;
    virtual int getClientIndex() const = 0;

protected:
    // Initializes socket.
    bool initializeSocket(std::string listenPort = "0");

    // Sending and receiving packets.
    bool sendPacket(PacketBase& packet, const sf::IpAddress& address, unsigned short port);
    bool receivePacket(std::unique_ptr<PacketBase>& packet, sf::IpAddress& address, unsigned short& port);

    // Sending and receiving data buffers.
    bool sendData(const MemoryBuffer& buffer, const sf::IpAddress& address, unsigned short port);
    bool receiveData(MemoryBuffer& buffer, sf::IpAddress& address, unsigned short& port);

protected:
    // Network socket.
    sf::UdpSocket m_socket;
    unsigned short m_listenPort;
};
