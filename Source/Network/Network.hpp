#pragma once

#include "Precompiled.hpp"

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

    // Sends packet through socket.
    bool sendPacket(const MemoryBuffer& buffer, const sf::IpAddress& address, unsigned short port);

    // Receives packet from socket.
    bool receivePacket(MemoryBuffer& buffer, sf::IpAddress& address, unsigned short& port);

protected:
    // Network socket.
    sf::UdpSocket m_socket;
    unsigned short m_listenPort;
};
