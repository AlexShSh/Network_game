#pragma once

#include <SFML/Network.hpp>

enum Dir
{
    DOWN = 0,
    LEFT,
    RIGHT,
    UP,
    NONE
};

const float player_speed = 0.2;
const float _animation_speed = 0.005;
const int _frame_amount = 3;
const int player_health = 5;

enum ObjectType
{
    EMPTY,
    PLAYER,
    ENEMY,
    BULLET
};