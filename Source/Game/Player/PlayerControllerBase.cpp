#include "Precompiled.hpp"
#include "PlayerControllerBase.hpp"

PlayerControllerBase::PlayerControllerBase()
{
}

PlayerControllerBase::~PlayerControllerBase()
{
}

bool PlayerControllerBase::handleEvent(const sf::Event& event)
{
    return true;
}

void PlayerControllerBase::tick(float timeDelta)
{
}
