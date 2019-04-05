#pragma once

#include "Precompiled.hpp"

extern ConsoleVariable<std::string> cv_port;
extern ConsoleVariable<std::string> cv_connect;

class Network
{
public:
    virtual bool initialize() = 0;
    virtual void onUpdate(float timeDelta) = 0;
    virtual void onTick(float timeDelta) = 0;

    virtual bool isConnected() const = 0;
    virtual bool isServer() const = 0;
    virtual bool isClient() const = 0;
    virtual int getClientIndex() const = 0;
};
