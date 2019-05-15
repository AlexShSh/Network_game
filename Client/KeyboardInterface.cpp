#include "KeyboardInterface.h"

Dir KeyboardInterface::get_direction()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            return UP_RIGHT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            return DOWN_RIGHT;
        else
            return RIGHT;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            return UP_LEFT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            return DOWN_LEFT;
        else
            return LEFT;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        return UP;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        return DOWN;
    }

    return NONE;
}