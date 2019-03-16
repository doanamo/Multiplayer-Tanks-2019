#include "Precompiled.hpp"
#include "Network/Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

bool Client::initialize()
{

    return true;
}

void Client::onUpdate(float timeDelta)
{
}

bool Client::isConnected() const
{
    return false;
}

bool Client::isServer() const
{
    return false;
}

bool Client::isClient() const
{
    return true;
}

int Client::getClientIndex() const
{
    return 0;
}

