#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"

class GameStateSession;

class GameStateEntry : public GameStateBase
{
public:
    GameStateEntry();
    ~GameStateEntry();

    // Initializes game state instance.
    bool initialize();

    // Game state methods.
    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;

protected:
    // Game state transitions.
    bool onStateEnter(State<GameStateBase>* previousState) override;
    bool onStateExit(State<GameStateBase>* newState) override;
};
