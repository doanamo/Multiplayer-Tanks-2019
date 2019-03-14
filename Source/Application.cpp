#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "Game/GameInstance.hpp"
#include "Game/PlayerController.hpp"
#include "Game/World.hpp"
#include "Game/Tank.hpp"
#include "Game/Level.hpp"

Application::Application() :
    m_gameInstance(nullptr)
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
}

bool Application::initialize()
{
    // Initialize game instance.
    m_gameInstance = new GameInstance();
    if(!m_gameInstance->initialize())
        return false;

    // Create player tank object.
    Tank* playerTank = new Tank();
    Handle playerHandle = m_gameInstance->getWorld()->addObject(playerTank);
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

void Application::update(float timeDelta)
{
    // Update game instance.
    m_gameInstance->update(timeDelta);
}

void Application::draw(float updateAlpha)
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

    sf::View viewport;
    viewport.setCenter(0.0f, 0.0f);
    viewport.setSize(viewportSize);
    g_window->render.setView(viewport);

    // Draw game instance.
    m_gameInstance->draw(updateAlpha);

    // Draw demo ImGui window.
    //ImGui::ShowDemoWindow();
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
    std::ofstream file("snapshot.save", std::ios::binary | std::ios::trunc);
    
    if(!file.is_open())
    {
        std::cout << "Could not open snapshot file for writing!" << std::endl;
        return false;
    }

    file.write(memoryBuffer.getData(), memoryBuffer.getSize());
    file.close();

    // Success!
    std::cout << "Snapshot file has been saved!" << std::endl;

    return true;
}

bool Application::loadSnaphot()
{
    // Open and read file into memory buffer.
    std::ifstream file("snapshot.save", std::ios::binary | std::ios::ate);

    if(!file.is_open())
    {
        std::cout << "Could not open snapshot file for reading!" << std::endl;
        return false;
    }

    std::size_t size = (std::size_t)file.tellg();
    file.seekg(0, std::ios::beg);

    // Read snapshot data into memory buffer.
    MemoryBuffer memoryBuffer;
    memoryBuffer.resize(size);

    if(!file.read(memoryBuffer.getData(), size))
    {
        std::cout << "Could not read snapshot file!" << std::endl;
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
    std::cout << "Snapshot file has been loaded!" << std::endl;

    return true;
}
