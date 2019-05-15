#pragma once

#include <SFML/Network.hpp>

enum Dir
{
    DOWN = 0,
    DOWN_RIGHT,
    RIGHT,
    UP_RIGHT,
    UP,
    UP_LEFT,
    LEFT,
    DOWN_LEFT,
    NONE
};

const float player_speed = 0.2;
const float _animation_speed = 0.1;
const int _frame_amount = 16;
const int player_health = 5;

enum ObjectType
{
    EMPTY,
    PLAYER,
    ENEMY,
    BULLET
};