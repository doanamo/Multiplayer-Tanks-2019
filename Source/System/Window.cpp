#include "Precompiled.hpp"
#include "Window.hpp"

ConsoleVariable<bool> cv_showImGuiDemo("showImGuiDemo", false);

Window::Window() :
    render(m_window)
{
}

Window::~Window()
{
    // Shutdown ImGui.
    ImGui::SFML::Shutdown();

    // Close window.
    m_window.close();
}

bool Window::initialize(int width, int height, const char* title)
{
    // Create SFML window.
    m_window.create(sf::VideoMode(width, height), title);

    if(!m_window.isOpen())
    {
        LOG_FATAL("Could not create SFML window!");
        return false;
    }

    m_title = title;

    // Limit framerate which can reach really high values in e.g. menus.
    m_window.setFramerateLimit(300);
    
    // Disable key repeats.
    m_window.setKeyRepeatEnabled(false);

    // Initialize ImGui.
    if(!ImGui::SFML::Init(m_window))
    {
        LOG_FATAL("ImGui initialization error!");
        return false;
    }

    // Reset the timer.
    m_clock.restart();

    return true;
}

void Window::close()
{
    m_window.close();
}

bool Window::pollEvent(sf::Event& event)
{
    while(m_window.pollEvent(event))
    {
        if(ImGui::SFML::ProcessEvent(event))
            continue;

        if(event.type == sf::Event::Closed)
        {
            m_window.close();
        }
        else if(event.type == sf::Event::Resized)
        {
            // Update ImGui display size.
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize.x = (float)event.size.width;
            io.DisplaySize.y = (float)event.size.height;

            LOG_INFO("Window resized to %ix%i.", event.size.width, event.size.height);
        }

        return true;
    }

    return false;
}

void Window::beginRender()
{
    // Update ImGui interface.
    sf::Time timeDelta = m_clock.restart();
    ImGui::SFML::Update(m_window, timeDelta);

    // Clear render target.
    m_window.clear();
}

void Window::endRender()
{
    // Show ImGui demo window.
    if(cv_showImGuiDemo.value)
    {
        ImGui::ShowDemoWindow(&cv_showImGuiDemo.value);
    }

    // Render ImGui interface.
    ImGui::SFML::Render(m_window);

    // Display render target.
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

void Window::setTitle(const sf::String title)
{
    m_window.setTitle(title);
}

const sf::String& Window::getInitialTitle() const
{
    return m_title;
}
