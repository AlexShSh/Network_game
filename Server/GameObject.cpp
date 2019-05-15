#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float x, float y, Dir dir_) :
    position(x, y),
    speed(player_speed),
    dir(dir_)
{}

GameObject::GameObject() :
    position(250, 250),
    speed(player_speed),
    dir(LEFT),
    collider()
{}

void GameObject::set_position(sf::Vector2f &pos)
{
    position = pos;
}

void GameObject::set_speed(float speed_)
{
    speed = speed_;
}

void GameObject::set_direction(Dir dir_)
{
    dir = dir_;
}

sf::Vector2f GameObject::get_position() const
{
    return position;
}

float GameObject::get_speed() const
{
    return speed;
}

Dir GameObject::get_direction() const
{
    return dir;
}

bool GameObject::update_from_packet(sf::Packet packet, sf::Time time)
{
    sf::Int16 dir_tmp = -1;
    if (!(packet >> dir_tmp))
        return false;

    dir = (Dir) dir_tmp;

    switch (dir)
    {
        case UP:
            position += {0, -speed * time.asMilliseconds()};
            break;
        case DOWN:
            position += {0, speed * time.asMilliseconds()};
            break;
        case LEFT:
            position += {-speed * time.asMilliseconds(), 0};
            break;
        case RIGHT:
            position += {speed * time.asMilliseconds(), 0};
    }

    return true;
}