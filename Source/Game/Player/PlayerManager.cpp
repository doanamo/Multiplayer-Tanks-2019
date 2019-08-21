#include "Precompiled.hpp"
#include "PlayerManager.hpp"
#include "Player.hpp"

PlayerManager::PlayerManager() :
    m_initialized(false)
{
}

PlayerManager::~PlayerManager()
{
}

bool PlayerManager::initialize(GameInstance* gameInstance)
{
    ASSERT(gameInstance);

    // Success!
    m_initialized = true;
    return true;
}

Player& PlayerManager::createPlayer()
{
    ASSERT(m_initialized);

    // Create new player entry.
    auto playerEntry = m_playerList.createHandle();
    ASSERT(playerEntry.valid == true, "Failed to create handle (invalid entry)!");
    ASSERT(playerEntry.value != nullptr, "Failed to create handle (nullptr entry)!");

    // Set handle in player entry.
    Player* player = playerEntry.value;
    player->setPlayerHandle(playerEntry.handle);

    // Return reference to player entry.
    return *playerEntry.value;
}

Player* PlayerManager::fetchPlayer(PlayerHandle playerHandle)
{
    ASSERT(m_initialized);

    // Retrieve and return player entry (which can be nullptr).
    auto playerEntry = m_playerList.fetchHandle(playerHandle);
    return playerEntry.value;
}

bool PlayerManager::removePlayer(PlayerHandle playerHandle)
{
    ASSERT(m_initialized);

    return m_playerList.removeHandle(playerHandle);
}

bool PlayerManager::handleEvent(const sf::Event& event)
{
    ASSERT(m_initialized);

    for(auto player : m_playerList)
    {
        ASSERT(player.value != nullptr, "Iterator is not supposed to return invalid entry!");
        if(!player.value->handleEvent(event))
            return false;
    }

    return true;
}

void PlayerManager::tick(float timeDelta)
{
    ASSERT(m_initialized);

    for(auto player : m_playerList)
    {
        ASSERT(player.value != nullptr, "Iterator is not supposed to return invalid entry!");
        player.value->tick(timeDelta);
    }
}

bool PlayerManager::onSerialize(MemoryStream& stream) const
{
    ASSERT(m_initialized);

    // #todo

    return true;
}

bool PlayerManager::onDeserialize(MemoryStream& stream)
{
    // #todo

    return true;
}
