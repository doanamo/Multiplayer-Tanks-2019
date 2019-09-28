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

void NetworkOffline::preTick(float timeDelta)
{
}

void NetworkOffline::postTick(float timeDelta)
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

bool NetworkOffline::isHost() const
{
    return true;
}
