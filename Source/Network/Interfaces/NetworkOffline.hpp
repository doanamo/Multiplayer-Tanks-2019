#pragma once

#include "Precompiled.hpp"
#include "NetworkInterface.hpp"

class NetworkOffline : public NetworkInterface
{
public:
    NetworkOffline();
    ~NetworkOffline();

    // Network methods.
    void update(float timeDelta) override;
    void preTick(float timeDelta) override;
    void postTick(float timeDelta) override;
    void draw() override;

    // Network info.
    NetworkMode getMode() const override;
    bool isConnected() const override;
};
