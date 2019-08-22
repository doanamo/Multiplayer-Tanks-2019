#include "Precompiled.hpp"
#include "PlayerManager.hpp"
#include "Player.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"

PlayerManager::PlayerManager() :
    m_gameInstance(nullptr),
    m_initialized(false)
{
}

PlayerManager::~PlayerManager()
{
}

bool PlayerManager::initialize(GameInstance* gameInstance)
{
    // Save game instance reference.
    if(gameInstance == nullptr)
        return false;

    m_gameInstance = gameInstance;

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

    // Process player input events.
    for(auto playerEntry : m_playerList)
    {
        // Get player from handle map entry.
        ASSERT(playerEntry.value != nullptr, "Iterator is not supposed to return invalid entry!");
        Player* player = playerEntry.value;
        
        // Get player controller attached to player entry.
        PlayerControllerBase* playerController = player->getPlayerController();

        if(playerController)
        {
            // Tick player controller.
            if(!playerController->handleEvent(event))
                return false;;
        }
    }

    return true;
}

void PlayerManager::tick(float timeDelta)
{
    ASSERT(m_initialized);

    // Retrieve world from game instance.
    World& world = m_gameInstance->getWorld();

    // Process player entries in tick.
    for(auto playerEntry : m_playerList)
    {
        // Get player from handle map entry.
        ASSERT(playerEntry.value != nullptr, "Iterator is not supposed to return invalid entry!");
        Player* player = playerEntry.value;

        // Get player controller attached to player entry.
        PlayerControllerBase* playerController = player->getPlayerController();

        if(playerController)
        {
            // Tick player controller.
            playerController->tick(timeDelta);

            // Retrieve controlled object.
            Object* object = world.getObjectByHandle(playerController->getControlledObject());

            // Pass player commands to controlled object.
            while(true)
            {
                PlayerCommand playerCommand = playerController->popPlayerCommand();
                if(playerCommand == PlayerCommand::Invalid)
                    break;

                if(object)
                {
                    object->onPlayerCommand(playerCommand);
                }
            }
        }
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
