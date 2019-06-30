#include "Precompiled.hpp"
#include "Application.hpp"
#include "System/Globals.hpp"
#include "System/Console.hpp"

Application::Application()
{
}

Application::~Application()
{
    LOG_TRACE("Application::~Application()");
}

bool Application::initialize()
{
    // Initialize game state instances.
    auto gameStateEntry = std::make_shared<GameStateEntry>();
    if(!gameStateEntry->initialize())
        return false;

    // Set initial game state.
    if(!m_gameStateMachine.changeState(gameStateEntry))
    {
        LOG_ERROR("Failed to set initial game state!");
        return false;
    }

    // Success!
    return true;
}

void Application::handleEvent(const sf::Event& event)
{
    // Handle console input.
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Tilde:
            cv_showConsole.value = !cv_showConsole.value;
            break;
        }
    }

    // Invoke current game state.
    auto currentState = m_gameStateMachine.getState();

    if(currentState)
    {
        currentState->handleEvent(event);
    }
}

void Application::update(float timeDelta)
{
    // Call game state method.
    auto currentState = m_gameStateMachine.getState();

    if(currentState)
    {
        currentState->update(timeDelta);
    }
}

void Application::tick(float timeDelta)
{
    // Invoke current game state.
    auto currentState = m_gameStateMachine.getState();

    if(currentState)
    {
        currentState->tick(timeDelta);
    }
}

void Application::draw(float timeAlpha)
{
    // Invoke current game state.
    auto currentState = m_gameStateMachine.getState();

    if(currentState)
    {
        currentState->draw(timeAlpha);
    }

    // Draw console.
    g_console->display();
}
