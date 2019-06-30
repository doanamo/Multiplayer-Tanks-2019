#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

enum class NetworkMode
{
    Offline,
    Server,
    Client,
};

struct NetworkParams
{
    NetworkParams() :
        mode(NetworkMode::Offline),
        address("127.0.0.1"),
        port(2077)
    {
    }

    NetworkMode mode;
    std::string address;
    unsigned short port;
};

class NetworkInterface
{
public:
    // Virtual destructor.
    virtual ~NetworkInterface()
    {
    }

    // Network methods.
    virtual void update(float timeDelta) = 0;
    virtual void tick(float timeDelta) = 0;
    virtual void draw() = 0;

    // Network info.
    virtual NetworkMode getMode() const = 0;
    virtual bool isConnected() const = 0;
};
