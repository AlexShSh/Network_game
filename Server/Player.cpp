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
    time_after_shoot(sf::milliseconds(conf::Player::shooting_delay))
{}

void Player::open_packet(sf::Packet packet)
{
    sf::Int16 dir_tmp = -1, is_shoot_tmp = 0;
    if (!(packet >> dir_tmp >> is_shoot_tmp))
    {
        std::cout << dir_tmp << " " << is_shoot_tmp << std::endl;
        moving_dir = conf::Dir::NONE;
    }
    else
    {
        moving_dir = (conf::Dir) dir_tmp;
        dir = (conf::Dir) dir_tmp;
        shoot_click = (bool) is_shoot_tmp;
    }
}

void Player::update(sf::Time time)
{
    if (!is_active)
        return;

    float tm = time.asMilliseconds();
    switch (moving_dir)
    {
        case conf::Dir::UP:
            position += {0, -speed * tm};
            animate(tm);
            break;
        case conf::Dir::DOWN:
            position += {0, speed * tm};
            animate(tm);
            break;
        case conf::Dir::LEFT:
            position += {-speed * tm, 0};
            animate(tm);
            break;
        case conf::Dir::RIGHT:
            position += {speed * tm, 0};
            animate(tm);
            break;
        case conf::Dir::UP_LEFT:
            position += {-diag_speed * tm, -diag_speed * tm};
            animate(tm);
            break;
        case conf::Dir::DOWN_LEFT:
            position += {-diag_speed * tm, diag_speed * tm};
            animate(tm);
            break;
        case conf::Dir::UP_RIGHT:
            position += {diag_speed * tm, -diag_speed * tm};
            animate(tm);
            break;
        case conf::Dir::DOWN_RIGHT:
            position += {diag_speed * tm, diag_speed * tm};
            animate(tm);
            break;
        default:
            break;
    }
    moving_dir = conf::Dir::NONE;

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