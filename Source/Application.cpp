#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.hpp"
#include "System/Console.hpp"
#include "Network/Server.hpp"
#include "Network/Client.hpp"
#include "Game/GameInstance.hpp"
#include "Game/PlayerController.hpp"
#include "Game/World.hpp"
#include "Game/Tank.hpp"
#include "Game/Level.hpp"

Application::Application() :
    m_network(nullptr),
    m_gameInstance(nullptr),
    isViewportCentered(false),
    isCameraAttachedToPlayer(true)
{
}

Application::~Application()
{
    // Shutdown game instance.
    if(m_gameInstance)
    {
        delete m_gameInstance;
        m_gameInstance = nullptr;
    }

    if(m_network)
    {
        delete m_network;
        m_network = nullptr;
    }
}

bool Application::initialize()
{
    // Create network interface.
    bool isServer = true;

    if(isServer)
    {
        m_network = new Server();
    }
    else
    {
        m_network = new Client();
    }

    // Change window title to identify used network interface.
#ifndef NDEBUG
    if(isServer)
    {
        g_window->setTitle(g_window->getInitialTitle() + " - Server");
    }
    else
    {
        g_window->setTitle(g_window->getInitialTitle() + " - Client " + std::to_string(m_network->getClientIndex()));
    }
#endif

    // Initialize game instance.
    m_gameInstance = new GameInstance();
    if(!m_gameInstance->initialize())
        return false;

    // Create player tank object.
    Tank* playerTank = new Tank();
    Handle playerHandle = m_gameInstance->getWorld()->addObject(playerTank, "Player1_Tank", "Players");
    m_gameInstance->getPlayerController()->control(playerHandle);

    // Test instantiation from runtime type.
    Object* enemyTank = Object::create(Tank::Type().getIdentifier());
    enemyTank->getTransform().setPosition(sf::Vector2f(0.0f, 2.0f));
    m_gameInstance->getWorld()->addObject(enemyTank);

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
            g_console->toggle();
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
            isCameraAttachedToPlayer = true;
            LOG_TRACE("Camera has re-attached to the player's tank.");
            break;

        case sf::Keyboard::I:
        case sf::Keyboard::K:
        case sf::Keyboard::J:
        case sf::Keyboard::L:
            if (isCameraAttachedToPlayer)
            {
                isCameraAttachedToPlayer = false;
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
    // Update game instance.
    m_gameInstance->update(timeDelta);
}

void Application::tick(float timeDelta)
{
    // Tick game instance.
    m_gameInstance->tick(timeDelta);
}

void Application::draw(float timeAlpha)
{
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

    if (!isViewportCentered)
    {
        viewport.setCenter(0.0f, 0.0f);
        isViewportCentered = true;
        LOG_TRACE("Application::draw - viewport centered.");
    }

    viewport.setSize(viewportSize);
    g_window->render.setView(viewport);

    // Manual camera navigation.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) viewport.move(0.f * timeAlpha, -0.05f * timeAlpha);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) viewport.move(0.f * timeAlpha, 0.05f * timeAlpha);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) viewport.move(-0.05f * timeAlpha, 0.f * timeAlpha);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) viewport.move(0.05f * timeAlpha, 0.f * timeAlpha);
     
    // Draw game instance.
    m_gameInstance->draw(timeAlpha);

    if (isCameraAttachedToPlayer)
    {
        Tank* tank = dynamic_cast<Tank*>(m_gameInstance->getWorld()->getObjectByName("Player1_Tank"));
        viewport.setCenter(tank->getPosition().x, tank->getPosition().y);
        // LOG_TRACE("X: %f Y: %f", tank->getPosition().x, tank->getPosition().y);
    }

    // Draw demo ImGui window.
    //ImGui::ShowDemoWindow();

    // Draw console.
    g_console->display();
}

bool Application::saveSnapshot()
{
    if(!m_gameInstance)
        return false;

    // Serialize game instance to memory buffer.
    MemoryBuffer memoryBuffer;
    if(!serialize(memoryBuffer, *m_gameInstance))
        return false;

    // Write buffer to disc.
    std::ofstream file("Snapshot.save", std::ios::binary | std::ios::trunc);
    
    if(!file.is_open())
    {
        LOG_ERROR("Could not open snapshot file for writing!");
        return false;
    }

    file.write(memoryBuffer.getData(), memoryBuffer.getSize());
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
    MemoryBuffer memoryBuffer;
    memoryBuffer.resize(size);

    if(!file.read(memoryBuffer.getData(), size))
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
