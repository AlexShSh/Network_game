#include <iostream>
#include "Player.h"
#include "Bullet.h"

Player::Player(float x, float y, conf::Dir dir_, ClientId id_) :
    GameObject(x, y, dir_, conf::Player::animation_speed, conf::Player::frame_amount,
              conf::Player::speed, conf::ObjectType::PLAYER),
    health(conf::Player::health),
    id(id_),
    moving_dir(conf::Dir::NONE),
    shoot_click(false),
    shoot_ready(false),
    time_after_shoot(sf::milliseconds(conf::Player::shooting_delay)),
    can_move(true)
{
    collider.set_size({conf::Player::obj_width, conf::Player::obj_height});
    collider.set_position(position);
}

void Player::open_packet(sf::Packet packet)
{
    if (!is_active)
        return;

    sf::Int16 dir_tmp = -1, is_shoot_tmp = 0;
    if (!(packet >> dir_tmp >> is_shoot_tmp))
    {
        moving_dir = conf::Dir::NONE;
    }
    else
    {
        moving_dir = (conf::Dir) dir_tmp;
        if (moving_dir != conf::Dir::NONE)
            dir = moving_dir;

        shoot_click = (bool) is_shoot_tmp;
    }
}

void Player::update(sf::Time time, std::list<GameObject*>& objects)
{
    if (!is_active)
        return;

    float tm = time.asMilliseconds();

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
        is_active = false;
    }

    time_after_shoot += time;
    if (shoot_click)
    {
        if (time_after_shoot.asMilliseconds() > conf::Player::shooting_delay)
        {
            shoot_ready = true;
            time_after_shoot = sf::milliseconds(0);
        }
        shoot_click = false;
    }
}

void Player::compress_packet(sf::Packet &packet)
{
    packet << (sf::Int16) type << id << position.x << position.y <<
          (sf::Int16) dir << get_current_frame() << health;
}

void Player::set_shoot_ready(bool st)
{
    shoot_ready = st;
}

bool Player::is_shoot() const
{
    return shoot_ready;
}

ClientId Player::get_id() const
{
    return id;
}


void Player::interract(std::list<GameObject *>& objects)
{
    for (auto obj : objects)
    {
        if (!obj->get_active())
            continue;

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

                if (this == bul->get_creator())
                {
                    continue;
                }

                bul->set_active(false);
                get_damage();
                break;
            }
            default:
                break;
        }
    }
}

void Player::get_damage()
{
    health--;
    std::cout << "!!!!!!!" << health << std::endl;
}

