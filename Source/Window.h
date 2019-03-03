#pragma once

#include "Precompiled.hpp"

class Window
{
public:
	Window();
	virtual ~Window();

private:
	char* windowName = "Tanks Project";
	unsigned const int windowWidth = 1024;
	unsigned const int windowHeight = 576;
	float runtime = 0;

public:
	sf::RenderWindow screen;
	sf::Clock timer;
	sf::Time elapsedTime;

private:
	void createSfmlWindow();

public:
	bool initializeImGui();
	void storeRuntime();
	void listenForCloseEvent();
	void clearAndUpdate();
	void renderAndDisplay();
	
public:
	float getRuntime();
};
