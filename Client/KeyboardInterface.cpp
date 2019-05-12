#include "KeyboardInterface.h"

Dir KeyboardInterface::get_direction()
{
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        return RIGHT;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
        return UP;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
        return DOWN;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
        return LEFT;

    return NONE;
}