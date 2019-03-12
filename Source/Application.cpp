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
    // Close application on F10 press.
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F10)
    {
        g_window->close();
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
