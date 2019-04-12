#pragma once

#include "Precompiled.hpp"
#include "Network/Network.hpp"

extern ConsoleVariable<std::string> cv_host;

class Server : public Network
{
public:
    Server();
    ~Server();

    bool initialize() override;
    void onUpdate(float timeDelta) override;
    void onTick(float timeDelta) override;

    bool isConnected() const override;
    bool isServer() const override;
    bool isClient() const override;
    int getClientIndex() const override;

private:
};
