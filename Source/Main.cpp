#include "Precompiled.hpp"

int main()
{
    // Create window.
    sf::RenderWindow window(sf::VideoMode(1024, 576), "Tanks Project");

    // Initialize ImGui.
    if(!ImGui::SFML::Init(window))
        return false;

    // Run main loop.
    sf::Clock timer;

    while(window.isOpen())
    {
        sf::Time elapsedTime = timer.restart();

        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, elapsedTime);
        ImGui::ShowDemoWindow(nullptr);

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    // Shutdown ImGui.
    ImGui::SFML::Shutdown();

    // Exit application.
    return 0;
}
