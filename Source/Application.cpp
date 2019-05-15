#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"
#include "System/Console.hpp"
#include "System/CommandLine.hpp"
#include "Game/GameInstance.hpp"
#include "Game/PlayerController.hpp"
#include "Game/World.hpp"
#include "Game/Tank.hpp"
#include "Game/Level.hpp"
#include "Network/Server.hpp"
#include "Network/Client.hpp"

ConsoleVariable<std::string> cv_address("address", "127.0.0.1");
ConsoleVariable<std::string> cv_port("port", "2076");

Application::Application() :
    m_gameInstance(nullptr),
    m_network(nullptr),
    m_isViewportCentered(false),
    m_isCameraAttachedToPlayer(true)
{
}

Application::~Application()
{
    // Shutdown network interface.
    if(m_network)
    {
        delete m_network;
        m_network = nullptr;
    }

    // Shutdown game instance.
    if(m_gameInstance)
    {
        delete m_gameInstance;
        m_gameInstance = nullptr;
    }
}

bool Application::initialize()
{
    // Initialize game instance.
    m_gameInstance = new GameInstance();
    if(!m_gameInstance->initialize())
        return false;

    // Create network interface.
    if(g_commandLine->hasArgument("host"))
    {
        // Create server network interface.
        m_network = new Server();

        // Change window title.
        g_window->setTitle(g_window->getInitialTitle() + " - Server");
    }
    else if(g_commandLine->hasArgument("connect"))
    {
        // Create client network interface.
        m_network = new Client();

        // Change window title.
        g_window->setTitle(g_window->getInitialTitle() + " - Client");
    }

    if(m_network)
    {
        // Parse port number string.
        unsigned short portNumber = 0;
        if(!parseStringToPort(cv_port.value, portNumber))
            return false;

        // Initialize network interface.
        if(!m_network->initialize(m_gameInstance, sf::IpAddress(cv_address.value), portNumber))
            return false;
    }

    // Prepare game instance on server.
    if(m_network && m_network->isServer())
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

    return true;
}

void Application::handleEvent(const sf::Event& event)
{
    // Handle keyboard input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Tilde:
            cv_showConsole.value = !cv_showConsole.value;
            break;

        case sf::Keyboard::F5:
            this->saveSnapshot();
            break;

        case sf::Keyboard::F8:
            this->loadSnaphot();
            break;

        case sf::Keyboard::F10:
            g_window->close();
            break;

        case sf::Keyboard::O:
            m_isCameraAttachedToPlayer = true;
            LOG_TRACE("Camera has re-attached to the player's tank.");
            break;

        case sf::Keyboard::I:
        case sf::Keyboard::K:
        case sf::Keyboard::J:
        case sf::Keyboard::L:
            if(m_isCameraAttachedToPlayer)
            {
                m_isCameraAttachedToPlayer = false;
                LOG_TRACE("Camera has been detached from the player. Press 'O' to restore camera to default.");
            }
            break;
        }
    }

    // Handle events by game instance.
    m_gameInstance->handleEvent(event);
}

void Application::update(float timeDelta)
{
    // Update network interface.
    if(m_network)
    {
        m_network->update(timeDelta);
    }

    // Update game instance.
    m_gameInstance->update(timeDelta);
}

void Application::tick(float timeDelta)
{
    // Tick network interface.
    if(m_network)
    {
        m_network->tick(timeDelta);
    }

    // Tick game instance.
    m_gameInstance->tick(timeDelta);
}

void Application::draw(float timeAlpha)
{
    ASSERT(timeAlpha >= 0.0f && timeAlpha <= 1.0f);

    // Calculate render target view size and aspect ratio.
    float horizontalAspectRatio = (float)g_window->getWidth() / (float)g_window->getHeight();
    float verticalAspectRatio = (float)g_window->getHeight() / (float)g_window->getWidth();

    if(horizontalAspectRatio >= verticalAspectRatio)
    {
        verticalAspectRatio = 1.0f;
    }
    else
    {
        horizontalAspectRatio = 1.0f;
    }

    // Setup camera viewport.
    sf::Vector2f viewportSize;
    viewportSize.x = 10.0f * horizontalAspectRatio;
    viewportSize.y = 10.0f * verticalAspectRatio;

    if(!m_isViewportCentered)
    {
        m_viewport.setCenter(0.0f, 0.0f);
        m_isViewportCentered = true;
        LOG_TRACE("Application::draw - viewport centered.");
    }

    m_viewport.setSize(viewportSize);
    g_window->render.setView(m_viewport);

    // Center camera on player.
    if(m_isCameraAttachedToPlayer)
    {
        Tank* tank = dynamic_cast<Tank*>(m_gameInstance->getWorld()->getObjectByName("Player1_Tank"));

        if(tank != nullptr)
        {
            sf::Vector2f interpolatedPosition = tank->getPosition(timeAlpha);
            m_viewport.setCenter(interpolatedPosition);
        }
    }

    // Manual camera navigation.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) m_viewport.move(0.f * timeAlpha, -0.05f * timeAlpha);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) m_viewport.move(0.f * timeAlpha, 0.05f * timeAlpha);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) m_viewport.move(-0.05f * timeAlpha, 0.f * timeAlpha);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::L)) m_viewport.move(0.05f * timeAlpha, 0.f * timeAlpha);

    // Draw game instance.
    m_gameInstance->draw(timeAlpha);

    // Draw network debug.
    if(m_network)
    {
        m_network->draw();
    }

    // Draw console.
    g_console->display();
}

bool Application::saveSnapshot()
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

bool Application::loadSnaphot()
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

    // Shutdown current game instance.
    if(m_gameInstance)
    {
        delete m_gameInstance;
        m_gameInstance = nullptr;
    }

    // Create new game instance.
    m_gameInstance = new GameInstance();
    if(!m_gameInstance->initialize())
        return false;

    // Deserialize game instance from memory buffer.
    if(!deserialize(memoryBuffer, m_gameInstance))
        return false;

    // Success!
    LOG_INFO("Snapshot file has been loaded!");

    return true;
}
