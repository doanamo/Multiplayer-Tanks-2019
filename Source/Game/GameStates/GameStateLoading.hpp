#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"

class GameStateLoading : public GameStateBase
{
public:
    GameStateLoading();
    ~GameStateLoading();

    // Base state interface.
    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;

protected:
    // State machine transitions.
    bool onStateEnter(State<GameStateBase>* newState) override;
    bool onStateExit(State<GameStateBase>* previousState) override;

private:
    // Temporary loading delay.
    float m_delay;
};
