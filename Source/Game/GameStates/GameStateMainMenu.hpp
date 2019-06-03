#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"

class GameStateMainMenu : public GameStateBase
{
public:
    GameStateMainMenu();
    ~GameStateMainMenu();

    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;

protected:
    bool onStateEnter(State<GameStateBase>* newState) override;
    bool onStateExit(State<GameStateBase>* previousState) override;

private:

};
