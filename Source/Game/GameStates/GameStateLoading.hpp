#pragma once

#include "Precompiled.hpp"
#include "GameStateBase.hpp"

class GameStateSession;

enum class GameProvisionMode
{
    Regular,
    LoadFromFile,
    LoadFromStream,
    Host,
    Connect,
};

struct GameProvisionParams
{
    GameProvisionParams();

    // Game instance provision mode.
    // Decides what will be done after creating new game instance.
    GameProvisionMode provisionMode;

    // Snapshot file to load game instance from.
    std::string snapshotFile;

    // Snapshot stream to load game instance from.
    MemoryStream* snapshotStream;

    // Network address used for connections.
    std::string connectionAddress;

    // Network port used for connection.
    unsigned short connectionPort;
};

class GameStateLoading : public GameStateBase
{
public:
    GameStateLoading(GameProvisionParams& params);
    ~GameStateLoading();

    // Base state interface.
    void handleEvent(const sf::Event& event) override;
    void update(float timeDelta) override;
    void tick(float timeDelta) override;
    void draw(float timeAlpha) override;

protected:
    // Prepares game session.
    bool provisionSession(std::shared_ptr<GameStateSession>& session);

    // State machine transitions.
    bool onStateEnter(State<GameStateBase>* newState) override;
    bool onStateExit(State<GameStateBase>* previousState) override;

private:
    // Game loading params.
    GameProvisionParams m_params;

    // Minimum time we want to spend on loading screen.
    float m_delay;
};
