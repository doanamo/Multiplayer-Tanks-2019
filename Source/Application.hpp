#pragma once

#include "Precompiled.hpp"
#include "Common/StateMachine.hpp"
#include "Game/GameStates/GameStateEntry.hpp"
#include "Game/GameStates/GameStateSession.hpp"

class Application
{
public:
    Application();
    ~Application();

    bool initialize();
    void handleEvent(const sf::Event& event);
    void update(float timeDelta);
    void tick(float timeDelta);
    void draw(float timeAlpha);

public:
    // Game state machine.
    StateMachine<GameStateBase> m_gameStateMachine;
};
