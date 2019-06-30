#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"
#include "Network/Interfaces/NetworkInterface.hpp"

class GameInstance;

class GameStateSession : public GameStateBase
{
public:
    GameStateSession();
    ~GameStateSession();

    // Initializes game state instance.
    bool initialize(const NetworkParams& networkParams);

    // Game state methods.
    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;
    
    // Returns the game instance.
    GameInstance* getGameInstance();

protected:
    // Game state transitions.
    bool onStateEnter(State<GameStateBase>* previousState) override;
    bool onStateExit(State<GameStateBase>* newState) override;

private:
    // Game instance.
    std::unique_ptr<GameInstance> m_gameInstance;

    // Game menu state.
    bool m_gameMenuOpen;
};
