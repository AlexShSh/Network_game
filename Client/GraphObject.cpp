#include "GraphObject.h"

#include <iostream>
#include <SFML/Graphics.hpp>

GraphObject::GraphObject(std::string file, int width_, int height_, float x, float y,
                         Dir dir, float animation_speed_, int frame_amount_) :
   width(width_),
   height(height_),
   sprite_coord(x, y),
   direction(dir),
   animation_speed(animation_speed_),
   frame_amount(frame_amount_)
{
    image.loadFromFile("../images/" + file);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
}

void GraphObject::set_position(float x, float y, Dir dir)
{
    sprite_coord = sf::Vector2f(x, y);
    direction = dir;
    sprite.setPosition(sprite_coord);
}

void GraphObject::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void GraphObject::frame_pos(Dir direct)
{
    sprite.setTextureRect(sf::IntRect(0, direct * height, width, height));
}

void GraphObject::animation(Dir direct, float time)
{
    current_frame += animation_speed * time;
    if (current_frame > frame_amount)
        current_frame = 0;

    sprite.setTextureRect(sf::IntRect(width * (int)current_frame, direct * height, width, height));
}
