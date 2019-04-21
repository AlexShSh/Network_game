#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"

enum Dir
{
    DOWN = 0, LEFT, RIGHT, UP
};

const float speed = 0.1;

class Player : public GraphObj
{
public:
    Player() = delete;
    Player(std::string file, int width, int height, float x, float y, float animation_speed, int frame_amount);
    int get_direction();
    void update(float time);
};