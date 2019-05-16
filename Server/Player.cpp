#include <iostream>
#include "Player.h"

Player::Player(float x, float y, conf::Dir dir_, ClientId id_) :
    GameObject(x, y, dir_, conf::Player::animation_speed, conf::Player::frame_amount,
              conf::Player::speed, conf::ObjectType::PLAYER),
    health(conf::Player::health),
    live(true),
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
    sf::Int16 dir_tmp = -1, is_shoot_tmp = 0;
    if (!(packet >> dir_tmp >> is_shoot_tmp))
    {
        //std::cout << dir_tmp << " " << is_shoot_tmp << std::endl;
        moving_dir = conf::Dir::NONE;
    }
    else
    {
        moving_dir = (conf::Dir) dir_tmp;
        dir = (conf::Dir) dir_tmp;
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
        sf::Vector2f shift = move(tm);

        position += shift;
        collider.set_position(position);
        interract(objects);

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
          (sf::Int16) dir << get_current_frame();
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

sf::Vector2f Player::move(float tm)
{
    switch (moving_dir)
    {
        case conf::Dir::UP:
            return  {0, -speed * tm};
        case conf::Dir::DOWN:
            return  {0, speed * tm};
        case conf::Dir::LEFT:
            return {-speed * tm, 0};
        case conf::Dir::RIGHT:
            return {speed * tm, 0};
        case conf::Dir::UP_LEFT:
            return {-diag_speed * tm, -diag_speed * tm};
        case conf::Dir::DOWN_LEFT:
            return {-diag_speed * tm, diag_speed * tm};
        case conf::Dir::UP_RIGHT:
            return  {diag_speed * tm, -diag_speed * tm};
        case conf::Dir::DOWN_RIGHT:
            return {diag_speed * tm, diag_speed * tm};
        default:
            return {0, 0};
    }
}

void Player::interract(std::list<GameObject *>& objects)
{
    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        auto obj = *it;
        auto type = obj->get_type();
        switch (type)
        {
            case conf::ObjectType::PLAYER:
            {
                auto plr = dynamic_cast<Player*>(obj);
                if (plr == this)
                    continue;

                if (!collider.detect_collision(plr->get_collider()))
                    continue;

                can_move = false;
                break;
            }
            default:
                break;
        }
    }
}