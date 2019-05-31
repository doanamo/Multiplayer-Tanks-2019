#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"

class Network;
class GameInstance;

class GameStateSession : public GameStateBase
{
public:
    GameStateSession();
    ~GameStateSession();

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

private:
    // Snapshot file save and loading.
    bool saveSnapshot();
    bool loadSnaphot();

private:
    // Game instance.
    std::unique_ptr<GameInstance> m_gameInstance;

    // Network interface.
    std::unique_ptr<Network> m_network;

    // Camera viewport
    sf::View m_viewport;

    bool m_isViewportCentered;
    bool m_isCameraAttachedToPlayer;
};