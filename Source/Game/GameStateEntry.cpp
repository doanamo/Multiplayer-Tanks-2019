#include "Precompiled.hpp"
#include "GameStateEntry.hpp"
#include "GameStateSession.hpp"

GameStateEntry::GameStateEntry() :
    m_gameStateSession(nullptr)
{
}

GameStateEntry::~GameStateEntry()
{
}

bool GameStateEntry::initialize(GameStateSession* gameStateSession)
{
    // Save game state transition reference.
    if(!gameStateSession)
    {
        LOG_ERROR("Game state transition reference cannot be null!");
        return false;
    }

    m_gameStateSession = gameStateSession;

    return true;
}

void GameStateEntry::handleEvent(const sf::Event& event)
{

}

void GameStateEntry::update(float timeDelta)
{

}

void GameStateEntry::tick(float timeDelta)
{

}

void GameStateEntry::draw(float timeAlpha)
{

}

bool GameStateEntry::onStateEnter(GameStateBase* previousState)
{
    // Change game state.
    auto stateMachine = getStateMachine();
    if(stateMachine == nullptr) return false;

    return stateMachine->changeState(m_gameStateSession);
}

bool GameStateEntry::onStateExit(GameStateBase* newState)
{
    return true;
}
