#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"

class GraphObject
{
public:
    GraphObject(std::string file, int width, int height, float x, float y,
                Dir dir, float animation_speed, int frame_amount);
    GraphObject() = default;
    void draw(sf::RenderWindow* window);
    void set_position(float x, float y, Dir dir);
    void frame_pos(Dir direct);
    void animation(Dir direct, float time);

protected:
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    int width;
    int height;
    sf::Vector2f sprite_coord;
    Dir direction;

    float animation_speed;
    float current_frame;
    int frame_amount;
};