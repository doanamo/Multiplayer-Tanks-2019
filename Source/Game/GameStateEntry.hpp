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
    bool initialize(GameStateSession* gameStateSession);

    // Game state methods.
    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;

protected:
    // Game state transitions.
    bool onStateEnter(GameStateBase* previousState) override;
    bool onStateExit(GameStateBase* newState) override;

private:
    // Game state transitions.
    GameStateSession* m_gameStateSession;
};
