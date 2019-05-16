#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float x, float y, conf::Dir dir_, float animation_speed_,
                       int frame_amount_, float speed_, conf::ObjectType type_) :
    position(x, y),
    speed(speed_),
    dir(dir_),
    animation_speed(animation_speed_),
    current_frame(0),
    frame_amount(frame_amount_),
    collider(),
    type(type_),
    is_active(true),
    diag_speed(0.7* speed)
{}

GameObject::GameObject() :
    position(250, 250),
    speed(0),
    dir(conf::LEFT),
    animation_speed(0),
    current_frame(0),
    frame_amount(0),
    collider(),
    type(conf::EMPTY),
    is_active(true),
    diag_speed(0.7* speed)
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

conf::ObjectType GameObject::get_type() const
{
    return type;
}

int GameObject::animate(float time)
{
    current_frame += animation_speed * time;
    if (current_frame > frame_amount)
        current_frame = 0;

    return (int) current_frame;
}

bool GameObject::get_active() const
{
    return is_active;
}