#include "Precompiled.hpp"

int main()
{
    // Create SFML window.
	Window window;
 
	// Initialize ImGui.
	window.initializeImGui();

	// Temp test shape.
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

    while(window.screen.isOpen())
    {
		window.storeRuntime();
		window.elapsedTime = window.timer.restart();

		window.listenForCloseEvent();

		window.clearAndUpdate();

		ImGui::ShowDemoWindow(nullptr);
		window.screen.draw(shape);

		window.renderAndDisplay();
    }

    // Shutdown ImGui.
    ImGui::SFML::Shutdown();

    // Exit application.
    return 0;
}
