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
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw() override;

    bool isConnected() const override;
    bool isServer() const override;
    bool isClient() const override;

private:
};
