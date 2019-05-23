#include "GraphObject.h"

#include <iostream>
#include <SFML/Graphics.hpp>

GraphObject::GraphObject(sf::Texture *texture_, int width_, int height_, float x, float y,
                         conf::Dir dir) :
    texture(texture_),
    width(width_),
    height(height_),
    sprite_coord(x, y),
    direction(dir),
    current_frame(0)
{
    sprite.setTextureRect(sf::IntRect(0, 0, width, height));
    sprite.setTexture(*texture);
}

void GraphObject::set_position(float x, float y, conf::Dir dir)
{
    sprite_coord = sf::Vector2f(x - width/2, y - height/2);
    direction = dir;
    sprite.setPosition(sprite_coord);
}

void GraphObject::draw(sf::RenderWindow* window)
{
    window->draw(sprite);
}

void GraphObject::frame_pos(conf::Dir direct, int _current_frame)
{
    current_frame = _current_frame;
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(width * (int) current_frame, direct * height, width, height));
}

void GraphObject::title(int x, int y)
{
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(x - 1 , y -1, width, height));
}