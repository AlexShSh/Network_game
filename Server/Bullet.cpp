#include "Bullet.h"
#include <iostream>

Bullet::Bullet(float x, float y, conf::Dir dir_, Player* creator_) :
    GameObject(x, y, dir_, conf::Bullet::animation_speed, conf::Bullet::frame_amount,
               conf::Bullet::speed, conf::ObjectType::BULLET),
    creator(creator_)
{
    collider.set_size({conf::Bullet::obj_width, conf::Bullet::obj_height});
    collider.set_position(position);
}

void Bullet::update(sf::Time time, std::list<GameObject*>& objects)
{
    if (!is_active)
        return;

    if (dir == conf::Dir::NONE)
    {
        is_active = false;
        return;
    }

    float tm = time.asMilliseconds();
    auto shift = get_shift(dir, tm);
    position += shift;
    collider.set_position(position);

    if (position.x < 0 || position.y < 0 ||
        position.x > conf::Map::width || position.y > conf::Map::height)
        is_active = false;
}

void Bullet::compress_packet(sf::Packet &packet)
{
    if (is_active)
        packet << (sf::Int16) type << 1 << position.x << position.y << (sf::Int16) dir << 1;
}

void Bullet::interract(std::list<GameObject *> &objects)
{
    for (auto obj : objects)
    {
        auto type = obj->get_type();
        if (type != conf::ObjectType::PLAYER)
            continue;

        if (!collider.detect_collision(obj->get_collider()))
            continue;

        if (obj == get_creator())
            continue;

        set_active(false);
    }
}

Player* Bullet::get_creator() const
{
    return creator;
}

void Bullet::set_creator(Player *pl)
{
    creator = pl;
}