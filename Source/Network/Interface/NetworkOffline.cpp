#include "Precompiled.hpp"
#include "NetworkOffline.hpp"

NetworkOffline::NetworkOffline()
{
}

NetworkOffline::~NetworkOffline()
{

}

void NetworkOffline::update(float timeDelta)
{
}

void NetworkOffline::tick(float timeDelta)
{
}

void NetworkOffline::draw()
{
}

NetworkMode NetworkOffline::getMode() const
{
    return NetworkMode::Offline;
}

bool NetworkOffline::isConnected() const
{
    return false;
}
