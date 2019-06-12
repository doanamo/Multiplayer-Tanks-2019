#include "Precompiled.hpp"
#include "GameStateSession.hpp"
#include "Game/GameInstance.hpp"
#include "Game/SnapshotSaveLoad.hpp"
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
    // Initialize game instance.
    m_gameInstance = std::make_unique<GameInstance>();
    if(!m_gameInstance->initialize())
        return false;

    return true;
}

bool GameStateSession::onStateEnter(State<GameStateBase>* previousState)
{
    return true;
}

bool GameStateSession::onStateExit(State<GameStateBase>* newState)
{
    // Clear game instance.
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
            // Save snapshot.
            {
                SnapshotSaveLoad snapshotSaver(m_gameInstance.get());
                snapshotSaver.save("Snapshot.save");
            }
            break;

        case sf::Keyboard::F8:
            // Load snapshot.
            m_gameInstance = std::make_unique<GameInstance>();

            if(m_gameInstance->initialize())
            {
                SnapshotSaveLoad snapshotLoader(m_gameInstance.get());
                snapshotLoader.load("Snapshot.save");
            }

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

GameInstance* GameStateSession::getGameInstance()
{
    return m_gameInstance.get();
}
