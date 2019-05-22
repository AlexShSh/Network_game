#include "GameObject.h"
#include <iostream>

GameObject::GameObject(float x, float y, conf::Dir dir_, float animation_speed_,
                       int frame_amount_, float speed_, conf::ObjectType type_) :
    position(x, y),
    speed(speed_),
    diag_speed(0.7f * speed),
    dir(dir_),
    animation_speed(animation_speed_),
    current_frame(0),
    frame_amount(frame_amount_),
    collider(),
    type(type_),
    is_active(true)
{}

GameObject::GameObject() :
    position(250, 250),
    speed(0),
    diag_speed(0.7f * speed),
    dir(conf::LEFT),
    animation_speed(0),
    current_frame(0),
    frame_amount(0),
    collider(),
    type(conf::EMPTY),
    is_active(true)
{}


void GameObject::set_position(sf::Vector2f &pos)
{
    position = pos;
    collider.set_position(pos);
}

void GameObject::set_speed(float speed_)
{
    speed = speed_;
}

void GameObject::set_direction(conf::Dir dir_)
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

conf::Dir GameObject::get_direction() const
{
    return dir;
}
int GameObject::get_current_frame() const
{
    return (int) current_frame;
}

conf::ObjectType GameObject::get_type() const
{
    return type;
}

int GameObject::animate(float time)
{
    current_frame += animation_speed * time;
    if (current_frame >= frame_amount)
        current_frame = 0;

    return (int) current_frame;
}

bool GameObject::get_active() const
{
    return is_active;
}

const RectangleCollider& GameObject::get_collider() const
{
    return collider;
}

sf::Vector2f GameObject::get_shift(conf::Dir dir, float tm)
{
    switch (dir)
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

void GameObject::set_active(bool act)
{
    is_active= act;
}

bool GameObject::check_border() const
{
    float left = position.x - collider.get_size().x / 2;
    float right = position.x + collider.get_size().x / 2;
    float top = position.y - collider.get_size().y / 2;
    float bottom = position.y + collider.get_size().y / 2;

    if (top < conf::Map::border_width ||
        bottom > conf::Map::height - conf::Map::border_width ||
        left < conf::Map::border_width ||
        right > conf::Map::width - conf::Map::border_width)
        return false;

    return true;
}

float GameObject::distance(sf::Vector2f pos)
{
    return (position.x - pos.x) * (position.x - pos.x) + (position.y - pos.y) * (position.y - pos.y);
}

void GameObject::get_damage(int size)
{}

float GameObject::fast_square_root(float n) {
    n = 1.0f / n;
    long i;
    float x, y;

    x = n * 0.5f;
    y = n;
    i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *) &i;
    y = y * (1.5f - (x * y * y));
    return y;
}

sf::Vector2f GameObject::compute_unit_vector(const sf::Vector2f &first, const sf::Vector2f &second)
{
    auto delta_x = first.x - second.x;
    auto delta_y = first.y - second.y;

    sf::Vector2f radius_vector(delta_x, delta_y);

    float vector_norm = fast_square_root((delta_x) * (delta_x) + (delta_y) * (delta_y));

    // normalize
    radius_vector.x = radius_vector.x / vector_norm;
    radius_vector.y = radius_vector.y / vector_norm;
    return {radius_vector.x, radius_vector.y};
}


sf::Vector2f operator*(const sf::Vector2f &first, float multiplier)
{
    return {first.x * multiplier, first.y * multiplier};
}