#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"

GraphObj::GraphObj(std::string file, int width, int height, float x, float y,
                   float animation_speed, int frame_amount)
{
    image.loadFromFile("images/" + file);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    width_ = width;
    height_ = height;
    sprite_coord = sf::Vector2f(x, y);
    animation_speed_ = animation_speed;
    frame_amount_ = frame_amount;
    direction = 0;
}

void GraphObj::move(float dx, float dy)
{
    sprite_coord += sf::Vector2f(dx, dy);
    sprite.setPosition(sprite_coord);
}

void GraphObj::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void GraphObj::frame_pos(int direct)
{
    sprite.setTextureRect(sf::IntRect(0, direct * height_, width_, height_));
}

void GraphObj::animation(int direct, float time)
{
    current_frame += animation_speed_ * time;
    if (current_frame > frame_amount_)
        current_frame = 0;

    sprite.setTextureRect(sf::IntRect(width_ * (int)current_frame, direct * height_, width_, height_));
}


