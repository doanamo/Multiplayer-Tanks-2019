#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Window.h"
#include "Game/WorldState.hpp"

Application::Application()
{
}

Application::~Application()
{
}

bool Application::initialize()
{
    // Create game state instance.
    m_gameState = new WorldState;

    if(!m_gameState->initialize())
    {
        shutdown();
        return false;
    }

    // Create render viewport.
    sf::Vector2u size = g_window->render.getSize();

    m_view.setCenter(0.0f, 0.0f);
    m_view.setSize((float)size.x, (float)size.y);
    g_window->render.setView(m_view);

    // Create player shape.
    m_gameState->playerShape.setFillColor(sf::Color::Green);
    m_gameState->playerShape.setSize(sf::Vector2f(32.0f, 32.0f));
    m_gameState->playerShape.setOrigin(sf::Vector2f(16.0f, 16.0f));

    return true;
}

void Application::shutdown()
{
    // Destroy game state instance.
    if(m_gameState)
    {
        m_gameState->shutdown();
        delete m_gameState;
        m_gameState = nullptr;
    }
}

void Application::handleEvent(sf::Event& event)
{
    if(event.type == sf::Event::Resized)
    {
        // Update view to match new window size.
        m_view.setCenter(0.0f, 0.0f);
        m_view.setSize((float)event.size.width, (float)event.size.height);
        g_window->render.setView(m_view);
    }
}

void Application::update(float timeDelta)
{
    // Save current player position.
    m_gameState->playerPreviousPosition = m_gameState->playerCurrentPosition;

    // Move player in direction.
    sf::Vector2f moveDirection;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveDirection.x -= 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveDirection.x += 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveDirection.y -= 1.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveDirection.y += 1.0f;

    m_gameState->playerCurrentPosition += moveDirection * timeDelta * 100.0f; // Todo: Normalize move direction.
}

void Application::draw(float updateAlpha)
{
    // Draw player shape.
    sf::Vector2f playerInterpolatedPosition = (1.0f - updateAlpha) * m_gameState->playerPreviousPosition + updateAlpha * m_gameState->playerCurrentPosition;
    m_gameState->playerShape.setPosition(playerInterpolatedPosition);

    g_window->render.draw(m_gameState->playerShape);

    // Draw demo ImGui window.
    ImGui::ShowDemoWindow();
}
