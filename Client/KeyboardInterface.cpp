#include "KeyboardInterface.h"
#include <iostream>

conf::Dir KeyboardInterface::get_direction()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            return conf::Dir::UP_RIGHT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            return conf::Dir::DOWN_RIGHT;
        else
            return conf::Dir::RIGHT;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            return conf::Dir::UP_LEFT;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            return conf::Dir::DOWN_LEFT;
        else
            return conf::Dir::LEFT;

    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        return conf::Dir::UP;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {

        return conf::Dir::DOWN;
    }

    return conf::Dir::NONE;
}

bool KeyboardInterface::get_shoot()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return true;

    return false;
}