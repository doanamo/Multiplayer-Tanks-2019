#include "Precompiled.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World.hpp"
#include "Game/Level.hpp"
#include "Game/PlayerController.hpp"

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
    // Update game level.
    m_level->update(timeDelta);

    // Update player controller.
    m_playerController->update(timeDelta);

    // Update world instance.
    m_world->update(timeDelta);
}

void GameInstance::draw(float updateAlpha)
{
    // Draw game level.
    m_level->draw(updateAlpha);

    // Draw world objects.
    m_world->draw(updateAlpha);
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
