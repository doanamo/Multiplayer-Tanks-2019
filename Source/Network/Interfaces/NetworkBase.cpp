#include "Precompiled.hpp"
#include "NetworkBase.hpp"
#include "Network/Packets/PacketHeader.hpp"
#include "Game/GameInstance.hpp"

ConsoleVariable<bool> cv_showNetwork("showNetwork", false);

NetworkBase::NetworkBase() :
    m_gameInstance(nullptr)
{
}

NetworkBase::~NetworkBase()
{
    LOG_TRACE("NetworkBase::~NetworkBase()");
}

bool NetworkBase::initialize(GameInstance* gameInstance)
{
    // Save game instance reference.
    if(gameInstance == nullptr)
    {
        LOG_ERROR("Game instance is needed to initialize network interface!");
        return false;
    }

    m_gameInstance = gameInstance;

    // Success!
    return true;
}

void NetworkBase::update(float timeDelta)
{
}

void NetworkBase::tick(float timeDelta)
{
}

void NetworkBase::draw()
{
}
