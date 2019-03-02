#include "Precompiled.hpp"

int main()
{
    // Create window.
    sf::RenderWindow window(sf::VideoMode(1024, 576), "Tanks Project");

    // Run main loop.
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }

    // Exit application.
    return 0;
}
