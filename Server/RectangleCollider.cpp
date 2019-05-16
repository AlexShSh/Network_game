#include "RectangleCollider.h"
#include <iostream>

RectangleCollider::RectangleCollider(sf::Vector2f &position, sf::Vector2f &size) :
    RectangleCollider()
{
    rectangle.setSize(size);
    rectangle.setPosition(position);
    rectangle.setFillColor(sf::Color::Transparent);
}

void RectangleCollider::set_position(sf::Vector2f &position)
{
    rectangle.setPosition(position);
}

void RectangleCollider::set_size(sf::Vector2f size)
{
    rectangle.setSize(size);
}

sf::Vector2f RectangleCollider::get_posotion() const
{
    return rectangle.getPosition();
}

sf::Vector2f RectangleCollider::get_size() const
{
    return rectangle.getSize();
}

bool RectangleCollider::detect_collision(const RectangleCollider &other) const
{
    sf::Vector2f this_size = this->get_size();
    sf::Vector2f this_pos  = this->get_posotion();
    sf::Vector2f oth_size = other.get_size();
    sf::Vector2f oth_pos  = other.get_posotion();

    float this_top = this_pos.y;
    float this_bottom = this_pos.y + this_size.y;
    float this_left = this_pos.x;
    float this_right = this_pos.x + this_size.x;

    float oth_top = oth_pos.y;
    float oth_bottom = oth_pos.y + oth_size.y;
    float oth_left = oth_pos.x;
    float oth_right = oth_pos.x + oth_size.x;

    if (this_bottom < oth_top ||
        this_top > oth_bottom ||
        this_left > oth_right ||
        this_right < oth_left)
        return false;

    return true;
}