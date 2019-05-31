#include "Precompiled.hpp"
#include "GameStateBase.hpp"

GameStateBase::GameStateBase()
{
}

bool GameStateBase::onStateEnter(State<GameStateBase>* newState)
{
    return true;
}

bool GameStateBase::onStateExit(State<GameStateBase>* previousState)
{
    return true;
}

void GameStateBase::handleEvent(const sf::Event& event)
{
}

void GameStateBase::update(float timeDelta)
{
}

void GameStateBase::tick(float timeDelta)
{
}

void GameStateBase::draw(float timeAlpha)
{
}
