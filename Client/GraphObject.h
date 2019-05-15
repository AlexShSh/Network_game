#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"

class GraphObject
{
public:
    GraphObject(sf::Texture *texture_, int width, int height, float x, float y,
                Dir dir);
    GraphObject() = default;
    void draw(sf::RenderWindow* window);
    void set_position(float x, float y, Dir dir);
    void frame_pos(Dir direct, int current_frame);
protected:
    sf::Texture *texture;
    sf::Sprite sprite;

    int width;
    int height;
    sf::Vector2f sprite_coord;
    Dir direction;
    float current_frame;

};