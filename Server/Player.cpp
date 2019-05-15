#include "Player.h"

Player::Player(float x, float y, conf::Dir dir_, ClientId id_) :
    GameObject(x, y, dir_, conf::animation_speed, conf::frame_amount, conf::ObjectType::PLAYER),
    health(conf::player_health),
    live(true),
    id(id_),
    moving_dir(conf::Dir::NONE)
{}

void Player::open_packet(sf::Packet packet)
{
    sf::Int16 dir_tmp = -1;
    if (!(packet >> dir_tmp))
    {
        moving_dir = conf::Dir::NONE;
    }
    else
    {
        moving_dir = (conf::Dir) dir_tmp;
        dir = (conf::Dir) dir_tmp;
    }
}

void Player::update(sf::Time time)
{
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
        default:
            break;
    }
    moving_dir = conf::Dir::NONE;
}

void Player::compress_packet(sf::Packet &packet)
{
    packet << (sf::Int16) type << id << position.x << position.y <<
          (sf::Int16) dir << get_current_frame();
}