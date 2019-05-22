#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Shared/GameConfig.h"

class GraphObject
{
public:
    GraphObject(sf::Texture *texture_, int width, int height, float x, float y,
                conf::Dir dir);

    GraphObject() = default;
    void draw(sf::RenderWindow* window);
    void set_position(float x, float y, conf::Dir dir);
    void frame_pos(conf::Dir direct, int current_frame);
    void title(int x, int y);

protected:
    sf::Texture *texture;
    sf::Sprite sprite;

    int width;
    int height;
    sf::Vector2f sprite_coord;
    conf::Dir direction;
    float current_frame;

};