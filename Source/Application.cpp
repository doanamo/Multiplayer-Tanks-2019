#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "Game/World.hpp"
#include "Game/Tank.hpp"

Application::Application() :
    m_world(nullptr),
    m_playerController(nullptr)
{
}

Application::~Application()
{
    // Shutdown systems in reverse order.
    if(m_playerController)
    {
        delete m_playerController;
        m_playerController = nullptr;
    }

    if(m_world)
    {
        delete m_world;
        m_world = nullptr;
    }
}

bool Application::initialize()
{
    // Create world instance.
    m_world = new World;

    if(!m_world->initialize())
        return false;

    // Create player controller.
    m_playerController = new PlayerController;

    if(!m_playerController->initialize(m_world))
        return false;

    // Create player tank object.
    Tank* playerTank = new Tank();
    Handle playerHandle = m_world->addObject(playerTank);
    m_playerController->control(playerHandle);

    return true;
}

void Application::handleEvent(const sf::Event& event)
{
    // Handle player controller input.
    m_playerController->onEvent(event);
}

void Application::update(float timeDelta)
{
    // Update player controller.
    m_playerController->onUpdate(timeDelta);

    // Update world instance.
    m_world->update(timeDelta);
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

    // Draw world objects.
    m_world->draw(updateAlpha);

    // Draw demo ImGui window.
    //ImGui::ShowDemoWindow();
}
