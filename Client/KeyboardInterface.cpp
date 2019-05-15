#include "KeyboardInterface.h"

conf::Dir KeyboardInterface::get_direction()
{
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        return conf::Dir::RIGHT;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
        return conf::Dir::UP;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
        return conf::Dir::DOWN;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
        return conf::Dir::LEFT;

    return conf::Dir::NONE;
}