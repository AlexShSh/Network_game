#pragma once

namespace conf
{

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


namespace Player
{
    const float speed = 0.2;
    const float animation_speed = 0.1;
    const int frame_amount = 16;
    const int health = 5;
    const int shooting_delay = 500;
    const int sprite_width = 64;
    const int sprite_height = 64;
    const int obj_width = 40;
    const int obj_height = 55;
}

namespace Bullet
{
    const float speed = 0.1;
    const float animation_speed = 0;
    const int frame_amount = 1;
}


enum ObjectType
{
    EMPTY,
    PLAYER,
    ENEMY,
    BULLET
};

namespace Map
{
    const int width = 600;
    const int height = 500;
}

}