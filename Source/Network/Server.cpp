#include "Precompiled.hpp"
#include "Network/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

bool Server::initialize()
{

    return true;
}

void Server::onUpdate(float timeDelta)
{
}

bool Server::isConnected() const
{
    return true;
}

bool Server::isServer() const
{
    return true;
}

bool Server::isClient() const
{
    return false;
}

int Server::getClientIndex() const
{
    return 0;
}

