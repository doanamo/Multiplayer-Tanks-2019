#include "Precompiled.hpp"
#include "GameStateEntry.hpp"
#include "GameStateMainMenu.hpp"

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
    // Create default state.
    auto defaultGameState = std::make_shared<GameStateMainMenu>();

    // Change game state.
    auto stateMachine = getStateMachine();
    if(stateMachine == nullptr)
        return false;

    return stateMachine->changeState(defaultGameState);
}

bool GameStateEntry::onStateExit(State<GameStateBase>* newState)
{
    return true;
}
