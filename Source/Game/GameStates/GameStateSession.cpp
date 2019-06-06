#include "Precompiled.hpp"
#include "GameStateSession.hpp"
#include "Game/GameInstance.hpp"
#include "Game/World/World.hpp"
#include "Game/Objects/Tank.hpp"
#include "Game/Level.hpp"
#include "Game/PlayerController.hpp"
#include "Network/Interface/NetworkInterface.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"

GameStateSession::GameStateSession()
{
}

GameStateSession::~GameStateSession()
{
}

bool GameStateSession::initialize()
{
    return true;
}

bool GameStateSession::onStateEnter(State<GameStateBase>* previousState)
{
    // Initialize game instance.
    m_gameInstance = std::make_unique<GameInstance>();
    if(!m_gameInstance->initialize())
        return false;

    // Prepare game instance on server.
    NetworkMode networkMode = m_gameInstance->getNetwork()->getMode();

    if(networkMode != NetworkMode::Client)
    {
        // Create player tank object.
        Tank* playerTank = new Tank();
        Handle playerHandle = m_gameInstance->getWorld()->addObject(playerTank, "Player1_Tank", "Players");
        m_gameInstance->getPlayerController()->control(playerHandle);

        // Test instantiation from runtime type.
        Object* enemyTank = Object::create(getTypeInfo<Tank>().getIdentifier());
        enemyTank->getTransform().setPosition(sf::Vector2f(0.0f, 2.0f));
        m_gameInstance->getWorld()->addObject(enemyTank);
    }

    // Success!
    return true;
}

bool GameStateSession::onStateExit(State<GameStateBase>* newState)
{
    // Clear allocation objects.
    m_gameInstance = nullptr;

    return true;
}

void GameStateSession::handleEvent(const sf::Event& event)
{
    // Handle keyboard input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::F5:
            this->saveSnapshot();
            break;

        case sf::Keyboard::F8:
            this->loadSnaphot();
            break;

        case sf::Keyboard::F10:
            g_window->close();
            break;
        }
    }

    // Handle events by game instance.
    m_gameInstance->handleEvent(event);
}

void GameStateSession::update(float timeDelta)
{
    // Update game instance.
    m_gameInstance->update(timeDelta);
}

void GameStateSession::tick(float timeDelta)
{
    // Tick game instance.
    m_gameInstance->tick(timeDelta);
}

void GameStateSession::draw(float timeAlpha)
{
    // Draw game instance.
    m_gameInstance->draw(timeAlpha);
}

bool GameStateSession::saveSnapshot()
{
    if(!m_gameInstance)
        return false;

    // Prepare world for saving.
    m_gameInstance->getWorld()->flushObjects();

    // Serialize game instance to memory buffer.
    MemoryStream memoryBuffer;
    if(!serialize(memoryBuffer, *m_gameInstance))
        return false;

    // Write buffer to disc.
    std::ofstream file("Snapshot.save", std::ios::binary | std::ios::trunc);

    if(!file.is_open())
    {
        LOG_ERROR("Could not open snapshot file for writing!");
        return false;
    }

    file.write(memoryBuffer.data(), memoryBuffer.size());
    file.close();

    // Success!
    LOG_INFO("Snapshot file has been saved!");

    return true;
}

bool GameStateSession::loadSnaphot()
{
    // Open and read file into memory buffer.
    std::ifstream file("Snapshot.save", std::ios::binary | std::ios::ate);

    if(!file.is_open())
    {
        LOG_ERROR("Could not open snapshot file for reading!");
        return false;
    }

    std::size_t size = (std::size_t)file.tellg();
    file.seekg(0, std::ios::beg);

    // Read snapshot data into memory buffer.
    MemoryStream memoryBuffer;
    memoryBuffer.resize(size);

    if(!file.read(memoryBuffer.data(), size))
    {
        LOG_ERROR("Could not read snapshot file!");
        return false;
    }

    file.close();

    // Create new game instance in place of old one.
    m_gameInstance = std::make_unique<GameInstance>();
    if(!m_gameInstance->initialize())
        return false;

    // Deserialize game instance from memory buffer.
    if(!deserialize(memoryBuffer, m_gameInstance.get()))
        return false;

    // Success!
    LOG_INFO("Snapshot file has been loaded!");

    return true;
}
