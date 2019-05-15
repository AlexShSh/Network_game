#include "KeyboardInterface.h"

conf::Dir KeyboardInterface::get_direction()
{
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        return conf::RIGHT;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
        return conf::UP;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
        return conf::DOWN;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
        return conf::LEFT;

    return conf::NONE;
}