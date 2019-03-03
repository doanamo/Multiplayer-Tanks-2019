#include "Precompiled.hpp"
#include "Window.h"

Window::Window()
{
	createSfmlWindow();
}

Window::~Window()
{

}

void Window::createSfmlWindow()
{
	screen.create(sf::VideoMode(windowWidth, windowHeight), windowName);
}

bool Window::initializeImGui()
{
	if (!ImGui::SFML::Init(screen))
	{
		std::cout << "ImGui initialization error!" << std::endl;
		return false;
	}
	return true;
}

void Window::storeRuntime()
{
	runtime += timer.getElapsedTime().asSeconds();
}

void Window::listenForCloseEvent()
{
	sf::Event event;
	while (screen.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
			screen.close();
	}
}

void Window::clearAndUpdate()
{
	screen.clear();
	ImGui::SFML::Update(screen, elapsedTime);
}

void Window::renderAndDisplay()
{
	ImGui::SFML::Render(screen);
	screen.display();
}

float Window::getRuntime() 
{ 
	return runtime; 
}