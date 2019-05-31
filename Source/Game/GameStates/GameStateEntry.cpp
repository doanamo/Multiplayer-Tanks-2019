#include "Precompiled.hpp"
#include "GameStateEntry.hpp"
#include "GameStateSession.hpp"

GameStateEntry::GameStateEntry()
{
}

GameStateEntry::~GameStateEntry()
{
}

bool GameStateEntry::initialize()
{
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

bool GameStateEntry::onStateEnter(State<GameStateBase>* previousState)
{
    // Create session game state.
    auto gameStateSession = std::make_shared<GameStateSession>();
    if(!gameStateSession->initialize())
        return false;

    // Change game state.
    auto stateMachine = getStateMachine();
    if(stateMachine == nullptr)
        return false;

    return stateMachine->changeState(gameStateSession);
}

bool GameStateEntry::onStateExit(State<GameStateBase>* newState)
{
    return true;
}
