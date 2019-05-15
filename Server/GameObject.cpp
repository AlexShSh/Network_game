#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float x, float y, Dir dir_, float animation_speed_, int frame_amount_, ObjectType type_) :
    position(x, y),
    speed(player_speed),
    dir(dir_),
    animation_speed(animation_speed_),
    frame_amount(frame_amount_),
    collider(),
    type(type_)
{
    diag_speed = 0.7 * player_speed;
}

GameObject::GameObject() :
    position(250, 250),
    speed(player_speed),
    dir(LEFT),
    animation_speed(_animation_speed),
    frame_amount(_frame_amount),

    collider(),
    type(EMPTY)

{
    diag_speed = 0.7 * player_speed;
}

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
int GameObject::get_current_frame() const 
{
    return current_frame;
}

int GameObject::animate(float time)
{
    
    current_frame += animation_speed * time;
    if (current_frame > frame_amount)
        current_frame = 0;
    return (int)current_frame;
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
            animate(time.asMilliseconds());
            break;
        case DOWN:
            position += {0, speed * time.asMilliseconds()};
            animate(time.asMilliseconds());
            break;
        case LEFT:
            position += {-speed * time.asMilliseconds(), 0};
            animate(time.asMilliseconds());
            break;
        case RIGHT:
            position += {speed * time.asMilliseconds(), 0};
            animate(time.asMilliseconds());
            break;
        case UP_LEFT:
            position += {-diag_speed * time.asMilliseconds(), -diag_speed * time.asMilliseconds()};
            animate(time.asMilliseconds());
            break;
        case DOWN_LEFT:
            position += {-diag_speed * time.asMilliseconds(), diag_speed * time.asMilliseconds()};
            animate(time.asMilliseconds());
            break;
        case UP_RIGHT:
            position += {diag_speed * time.asMilliseconds(), -diag_speed * time.asMilliseconds()};
            animate(time.asMilliseconds());
            break;
        case DOWN_RIGHT:
            position += {diag_speed * time.asMilliseconds(), diag_speed * time.asMilliseconds()};
            animate(time.asMilliseconds());
            break;
    }

    return true;
}