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
    bool onStateEnter(GameStateBase* previousState) override;
    bool onStateExit(GameStateBase* newState) override;

private:
    // Snapshot file save and loading.
    bool saveSnapshot();
    bool loadSnaphot();

private:
    // Game instance.
    GameInstance* m_gameInstance;

    // Network interface.
    Network* m_network;

    // Camera viewport
    sf::View m_viewport;

    bool m_isViewportCentered;
    bool m_isCameraAttachedToPlayer;
};
