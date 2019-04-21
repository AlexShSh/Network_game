#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "View.h"
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 500), "Let's work!");
    sf::Clock clock;
    view.reset(sf::FloatRect(0, 0, 600, 500));

    auto model = new Player("hero.png", 96, 96, 250, 250, 0.005, 3);


    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 1000;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        model->update(time);
        window.clear();
        model->draw(window);
        window.display();
    }
    return 0;
}