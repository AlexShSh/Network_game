#include "GameObject.h"

GameObject::GameObject(sf::Vector2f pos, float speed_, Dir dir_) :
    position(pos),
    speed(speed_),
    dir(dir_)
{}

GameObject::GameObject() :
    position({0, 0}),
    speed(PLAYER_SPEED),
    dir(LEFT)
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
    Dir new_dir;
    if (!(packet >> new_dir))
        return false;

    dir = new_dir;
    switch (dir)
    {
        case UP:
            position += {0, speed * time.asMilliseconds()};
            break;
        case DOWN:
            position += {0, -speed * time.asMilliseconds()};
            break;
        case LEFT:
            position += {-speed * time.asMilliseconds(), 0};
            break;
        case RIGHT:
            position += {speed * time.asMilliseconds(), 0};
    }
    return true;
}