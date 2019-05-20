#include <iostream>
#include "Enemy.h"
#include "Bullet.h"

Enemy::Enemy(float x, float y, conf::Dir dir, int number) :
        GameObject(x, y, dir, conf::Enemy::animation_speed, conf::Enemy::frame_amount,
                   conf::Enemy::speed, conf::ObjectType::ENEMY),
        health(conf::Enemy::health),
        number(number),
        moving_dir(conf::Dir::NONE),
        can_move(true),
        death(false)
{
    collider.set_size({conf::Enemy::obj_width, conf::Enemy::obj_height});
    collider.set_position(position);
}

int Enemy::get_number() const
{
    return number;
}

void Enemy::interract(std::list<GameObject *>& objects)
{
    for (auto obj : objects)
    {
        if (obj == this)
            continue;

        auto type = obj->get_type();
        if (!collider.detect_collision(obj->get_collider()))
            continue;

        switch (type)
        {
            case conf::ObjectType::PLAYER:
            {
                can_move = false;
                break;
            }
            case conf::ObjectType::BULLET:
            {
                auto bul = dynamic_cast<Bullet*>(obj);

                bul->set_active(false);
                get_damage();
                break;
            }
            case conf::ObjectType::ENEMY:
            {
                can_move = false;
                break;
            }
            default:
                break;
        }
    }
}

void Enemy::get_damage()
{
    health--;
}

void Enemy::update(sf::Time time, std::list<GameObject*>& objects)
{
    if (death)
    {
        is_active = false;
        return;
    }


    float tm = time.asMilliseconds();
    moving_dir = get_direction(objects);
    dir = moving_dir;

    if (moving_dir != conf::Dir::NONE)
    {
        sf::Vector2f shift = get_shift(moving_dir, tm);

        position += shift;
        collider.set_position(position);
        interract(objects);

        if(!check_border())
            can_move = false;

        if (!can_move)
        {
            position -= shift;
            can_move = true;
        } else
        {
            animate(tm);
        }
        collider.set_position(position);
        moving_dir = conf::Dir::NONE;
    }
    else
    {
        interract(objects);
    }

    if (health <= 0)
    {
        death = true;
        dir = conf::Dir::NONE;
    }
}

void Enemy::compress_packet(sf::Packet &packet)
{
    packet << (sf::Int16) type << number << position.x << position.y <<
           (sf::Int16) dir << get_current_frame();
}

conf::Dir Enemy::get_direction(std::list<GameObject *> &objects)
{
    float min_dist = 0;
    sf::Vector2f player_position;
    float player_dist;
    for (auto obj : objects)
    {
        auto type = obj->get_type();

        if(type != conf::ObjectType::PLAYER)
            continue;

        player_dist = distance(obj->get_position());

        if(min_dist == 0)
        {
            min_dist = player_dist;
            player_position = obj->get_position();
            continue;
        }

        if(player_dist < min_dist)
        {
            min_dist = player_dist;
            player_position = obj->get_position();
        }
    }

    if((position.x - player_position.x) > 32)
    {
        if((position.y - player_position.y) > 32)
            return conf::Dir::UP_LEFT;
        else if((position.y - player_position.y) < -32)
            return conf::Dir::DOWN_LEFT;
        else
            return conf::Dir::LEFT;
    }
    else if((position.x - player_position.x) < -32)
    {
        if((position.y - player_position.y) > 32)
            return conf::Dir::UP_RIGHT;
        else if((position.y - player_position.y) < -32)
            return conf::Dir::DOWN_RIGHT;
        else
            return conf::Dir::RIGHT;
    }
    else
    {
        if((position.y - player_position.y) > 0)
            return conf::Dir::UP;
        else
            return conf::Dir::DOWN;
    }
}