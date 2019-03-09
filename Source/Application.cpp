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
}

bool Application::initialize()
{
    // Create world instance.
    m_world = new World;

    if(!m_world->initialize())
    {
        shutdown();
        return false;
    }

    // Create player controller.
    m_playerController = new PlayerController;

    if(!m_playerController->initialize(m_world))
    {
        shutdown();
        return false;
    }

    // Create player tank object.
    Tank* tankObject = new Tank();
    Handle playerHandle = m_world->addObject(tankObject);
    m_playerController->control(playerHandle);

    return true;
}

void Application::shutdown()
{
    // Shutdown objects in reverse order.
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
    // Setup camera view.
    sf::View view;
    view.setCenter(0.0f, 0.0f);
    view.setSize((float)g_window->getWidth(), (float)g_window->getHeight());
    g_window->render.setView(view);

    // Draw the world.
    m_world->draw(updateAlpha);

    // Draw demo ImGui window.
    //ImGui::ShowDemoWindow();
}
