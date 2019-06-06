#pragma once

#include "Precompiled.hpp"
#include "Network/Packets/PacketBase.hpp"

enum class NetworkMode
{
    Offline,
    Server,
    Client,
};

class NetworkInterface
{
public:
    // Network methods.
    virtual void update(float timeDelta) = 0;
    virtual void tick(float timeDelta) = 0;
    virtual void draw() = 0;

    // Network info.
    virtual NetworkMode getMode() const = 0;
    virtual bool isConnected() const = 0;
};
