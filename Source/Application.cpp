#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "Game/World.hpp"
#include "Game/Tank.hpp"

Application::Application() :
    m_playerMovement(PlayerMovement::None)
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
    m_player = m_world->addObject(tankObject);

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

void Application::handleEvent(const sf::Event& event)
{
    // Handle player tank input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Key::Up:
            m_playerMovement = PlayerMovement::Up;
            break;

        case sf::Keyboard::Key::Down:
            m_playerMovement = PlayerMovement::Down;
            break;

        case sf::Keyboard::Key::Left:
            m_playerMovement = PlayerMovement::Left;
            break;

        case sf::Keyboard::Key::Right:
            m_playerMovement = PlayerMovement::Right;
            break;
        }
    }
}

void Application::update(float timeDelta)
{
    // Update the world.
    m_world->update(timeDelta);

    // Handle player movement input.
    Tank* playerTank = dynamic_cast<Tank*>(m_world->getObject(m_player));

    if(playerTank)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (m_playerMovement == PlayerMovement::Up || m_playerMovement == PlayerMovement::None))
        {
            playerTank->SetMovementInput(sf::Vector2f(0.0f, -1.0f));
            m_playerMovement = PlayerMovement::Up;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (m_playerMovement == PlayerMovement::Down || m_playerMovement == PlayerMovement::None))
        {
            playerTank->SetMovementInput(sf::Vector2f(.0f, 1.0f));
            m_playerMovement = PlayerMovement::Down;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (m_playerMovement == PlayerMovement::Left || m_playerMovement == PlayerMovement::None))
        {
            playerTank->SetMovementInput(sf::Vector2f(-1.0f, 0.0f));
            m_playerMovement = PlayerMovement::Left;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (m_playerMovement == PlayerMovement::Right || m_playerMovement == PlayerMovement::None))
        {
            playerTank->SetMovementInput(sf::Vector2f(1.0f, 0.0f));
            m_playerMovement = PlayerMovement::Right;
        }
        else
        {
            m_playerMovement = PlayerMovement::None;
        }
    }
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
