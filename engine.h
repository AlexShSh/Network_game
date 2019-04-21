#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class GraphObj
{
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    int width_, height_;
    sf::Vector2f sprite_coord;

    float animation_speed_;
    float current_frame;
    int frame_amount_;

public:
    GraphObj(std::string file, int width, int height, float x, float y, float animation_speed, int frame_amount);
    GraphObj() = default;
    void draw(sf::RenderWindow &window);
    void move(float dx, float dy);
    void frame_pos(int direct);
    void animation(int direct, float time);
    int direction;

};

