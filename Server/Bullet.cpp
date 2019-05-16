#include "Bullet.h"

Bullet::Bullet(float x, float y, conf::Dir dir_) :
    GameObject(x, y, dir_, conf::Bullet::animation_speed, conf::Bullet::frame_amount,
               conf::Bullet::speed, conf::ObjectType::BULLET)
{}

void Bullet::update(sf::Time time)
{
    if (!is_active)
        return;

    float tm = time.asMilliseconds();
    switch (dir)
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

    if (position.x < 0 || position.y < 0 ||
        position.x > conf::Map::width || position.y > conf::Map::height)
        is_active = false;
}

void Bullet::compress_packet(sf::Packet &packet)
{
    if (is_active)
        packet << (sf::Int16) type << 1 << position.x << position.y << (sf::Int16) dir << 1;
}