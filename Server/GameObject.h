#pragma once

#include <SFML/Network.hpp>

#include "../Shared/GameConfig.h"
#include "RectangleCollider.h"


class GameObject
{
public:
    GameObject(float x, float y, conf::Dir dir_, float animation_speed_,
               int frame_amount_, float speed_, conf::ObjectType type_);
    GameObject();
    virtual ~GameObject() = default;
    void set_position(sf::Vector2f& pos);
    void set_direction(conf::Dir dir_);
    void set_speed(float speed);
    int animate(float time);

    virtual void update(sf::Time time) = 0;
    virtual void compress_packet(sf::Packet& packet) = 0;

    sf::Vector2f get_position() const;
    conf::Dir get_direction() const;
    float get_speed() const;
    conf::ObjectType get_type() const;
    int get_current_frame() const;
    bool get_active() const;

protected:
    sf::Vector2f position;
    float speed;
    conf::Dir dir;

    float animation_speed;
    float current_frame;
    int frame_amount;

    RectangleCollider collider;
    conf::ObjectType type;

    bool is_active;
};
