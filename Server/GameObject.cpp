#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float x, float y, conf::Dir dir_, float animation_speed_, int frame_amount_, conf::ObjectType type_) :
    position(x, y),
    speed(conf::player_speed),
    dir(dir_),
    animation_speed(animation_speed_),
    frame_amount(frame_amount_),
    collider(),
    type(type_)
{}

GameObject::GameObject() :
    position(250, 250),
    speed(conf::player_speed),
    dir(conf::LEFT),
    animation_speed(conf::animation_speed),
    frame_amount(conf::frame_amount),
    collider(),
    type(conf::EMPTY)
{}

void GameObject::set_position(sf::Vector2f &pos)
{
    position = pos;
}

void GameObject::set_speed(float speed_)
{
    speed = speed_;
}

void GameObject::set_direction(conf::Dir dir_)
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

conf::Dir GameObject::get_direction() const
{
    return dir;
}
int GameObject::get_current_frame() const
{
    return (int) current_frame;
}

int GameObject::animate(float time)
{
    current_frame += animation_speed * time;
    if (current_frame > frame_amount)
        current_frame = 0;

    return (int) current_frame;
}
