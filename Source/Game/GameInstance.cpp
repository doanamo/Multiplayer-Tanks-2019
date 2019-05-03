#include "Precompiled.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World.hpp"
#include "Game/Level.hpp"
#include "Game/PlayerController.hpp"
#include "System/Window.hpp"

GameInstance::GameInstance() :
    m_world(nullptr),
    m_level(nullptr),
    m_playerController(nullptr)
{
}

GameInstance::~GameInstance()
{
    // Shutdown systems in reverse order.
    if(m_playerController)
    {
        delete m_playerController;
        m_playerController = nullptr;
    }

    if(m_level)
    {
        delete m_level;
        m_level = nullptr;
    }

    if(m_world)
    {
        delete m_world;
        m_world = nullptr;
    }
}

bool GameInstance::initialize()
{
    // Create world instance.
    m_world = new World;
    if(!m_world->initialize())
        return false;

    // Create game level.
    m_level = new Level;
    if(!m_level->initialize())
        return false;

    // Create player controller. 
    m_playerController = new PlayerController;
    if(!m_playerController->initialize(m_world))
        return false;

    return true;
}

void GameInstance::handleEvent(const sf::Event& event)
{
    // Handle player controller input.
    m_playerController->handleEvent(event);
}

void GameInstance::update(float timeDelta)
{
    // Update world instance.
    m_world->update(timeDelta);
}

void GameInstance::tick(float timeDelta)
{
    // Tick game level.
    m_level->tick(timeDelta);

    // Tick player controller.
    m_playerController->tick(timeDelta);

    // Tick world instance.
    m_world->tick(timeDelta);
}

void GameInstance::draw(float timeAlpha)
{
    // Draw game level.
    m_level->draw(timeAlpha);

    // Draw world objects.
    m_world->draw(timeAlpha);
}

World* GameInstance::getWorld()
{
    return m_world;
}

Level* GameInstance::getLevel()
{
    return m_level;
}

PlayerController* GameInstance::getPlayerController()
{
    return m_playerController;
}

bool GameInstance::onSerialize(MemoryBuffer& buffer)
{
    if(!serialize(buffer, *m_world))
        return false;

    if(!serialize(buffer, *m_level))
        return false;

    if(!serialize(buffer, *m_playerController))
        return false;

    return true;
}

bool GameInstance::onDeserialize(MemoryBuffer& buffer)
{
    if(!deserialize(buffer, m_world))
        return false;

    if(!deserialize(buffer, m_level))
        return false;

    if(!deserialize(buffer, m_playerController))
        return false;

    return true;
}
