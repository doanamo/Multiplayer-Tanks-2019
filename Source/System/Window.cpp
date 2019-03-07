#include "Precompiled.hpp"
#include "Window.h"

Window::Window() :
    render(m_window)
{
}

Window::~Window()
{
}

bool Window::initialize()
{
    // Create SFML window.
    m_window.create(sf::VideoMode(m_widht, m_height), m_name);

    if(!m_window.isOpen())
    {
        std::cout << "Could not create SFML window!" << std::endl;
        return false;
    }
    
    // Disable key repeats.
    m_window.setKeyRepeatEnabled(false);

    // Initialize ImGui.
    if(!ImGui::SFML::Init(m_window))
    {
        std::cout << "ImGui initialization error!" << std::endl;
        return false;
    }

    // Reset the timer.
    m_clock.restart();

    return true;
}

void Window::shutdown()
{
    m_window.close();
}

bool Window::pollEvent(sf::Event& event)
{
    if(m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if(event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        return true;
    }
    else
    {
        return false;
    }
}

void Window::beginRender()
{
    sf::Time elapsedTime = m_clock.restart();
    ImGui::SFML::Update(m_window, elapsedTime);
    m_window.clear();
}

void Window::endRender()
{
    ImGui::SFML::Render(m_window);
    m_window.display();
}

bool Window::isOpen() const
{
    return m_window.isOpen();
}

int Window::getWidth() const
{
    return m_window.getSize().x;
}

int Window::getHeight() const
{
    return m_window.getSize().y;
}
