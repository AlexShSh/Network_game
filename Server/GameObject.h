#pragma once

#include <SFML/Network.hpp>

#include "../Shared/GameConfig.h"
#include "RectangleCollider.h"


class GameObject
{
public:
    GameObject(float x, float y, Dir dir_);
    GameObject();
    void set_position(sf::Vector2f& pos);
    void set_direction(Dir dir_);
    void set_speed(float speed);
    bool update_from_packet(sf::Packet packet, sf::Time time);

    sf::Vector2f get_position() const;
    Dir get_direction() const;
    float get_speed() const;

protected:
    sf::Vector2f position;
    float speed;
    Dir dir;
    RectangleCollider collider;
};
