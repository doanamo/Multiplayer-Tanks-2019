#pragma once

#include "Precompiled.hpp"
#include "Common/HandleMap.hpp"
#include "Player.hpp"

// Forward declarations.
class GameInstance;

// Player manager class.
class PlayerManager : public Serializable
{
public:
    // Type declarations.
    using PlayerList = HandleMap<Player>;

public:
    PlayerManager();
    ~PlayerManager();

    // Initializes the player manager.
    bool initialize(GameInstance* gameInstance);

    // Creates new player.
    Player& createPlayer(const PlayerHandle& requestedHandle = PlayerHandle());

    // Fetches player by index.
    Player* fetchPlayer(const PlayerHandle& playerHandle);

    // Remove player.
    bool removePlayer(const PlayerHandle& playerHandler);

    // Handle player input events.
    bool handleEvent(const sf::Event& event);

    // Ticks players.
    void tick(float timeDelta);

private:
    // Player manager serialization.
    bool onSerialize(MemoryStream& stream) const override;
    bool onDeserialize(MemoryStream& stream) override;

private:
    // Game instance reference.
    GameInstance* m_gameInstance;

    // List of player entries.
    PlayerList m_playerList;

    // Initialization state.
    bool m_initialized;
};
