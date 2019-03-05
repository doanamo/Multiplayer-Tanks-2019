#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "Game/World.hpp"
#include "Game/Tank.hpp"

Application::Application()
{
}

Application::~Application()
{
}

bool Application::initialize()
{
    // Create game state instance.
    m_world = new World;

    if(!m_world->initialize())
    {
        shutdown();
        return false;
    }

    // Add tank object to the world.
    Tank* tankObject = new Tank();
    m_world->addObject(tankObject);

    return true;
}

void Application::shutdown()
{
    // Destroy game state instance.
    if(m_world)
    {
        m_world->shutdown();
        delete m_world;
        m_world = nullptr;
    }
}

void Application::handleEvent(sf::Event& event)
{
}

void Application::update(float timeDelta)
{
    // Update the world.
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
    ImGui::ShowDemoWindow();
}
