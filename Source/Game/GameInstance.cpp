#include "Precompiled.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"
#include "Game/Level.hpp"
#include "Game/PlayerController.hpp"
#include "System/Window.hpp"

GameInstance::GameInstance() :
    m_tickFrame(0),
    m_world(nullptr),
    m_level(nullptr),
    m_playerController(nullptr)
{
}

GameInstance::~GameInstance()
{
    // Shutdown systems in reverse order.
    m_playerController = nullptr;
    m_level = nullptr;
    m_world = nullptr;
}

bool GameInstance::initialize()
{
    // Create world instance.
    m_world = std::make_unique<World>();
    if(!m_world->initialize())
        return false;

    // Create game level.
    m_level = std::make_unique<Level>();
    if(!m_level->initialize())
        return false;

    // Create player controller. 
    m_playerController = std::make_unique<PlayerController>();
    if(!m_playerController->initialize(m_world.get()))
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
    // Increment total tick count.
    ++m_tickFrame;

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

uint64_t GameInstance::getTickFrame() const
{
    return m_tickFrame;
}

World* GameInstance::getWorld()
{
    return m_world.get();
}

Level* GameInstance::getLevel()
{
    return m_level.get();
}

PlayerController* GameInstance::getPlayerController()
{
    return m_playerController.get();
}

bool GameInstance::onSerialize(MemoryStream& buffer) const
{
    if(!serialize(buffer, m_tickFrame))
        return false;

    if(!serialize(buffer, *m_world))
        return false;

    if(!serialize(buffer, *m_level))
        return false;

    if(!serialize(buffer, *m_playerController))
        return false;

    return true;
}

bool GameInstance::onDeserialize(MemoryStream& buffer)
{
    if(!deserialize(buffer, &m_tickFrame))
        return false;

    if(!deserialize(buffer, m_world.get()))
        return false;

    if(!deserialize(buffer, m_level.get()))
        return false;

    if(!deserialize(buffer, m_playerController.get()))
        return false;

    return true;
}
