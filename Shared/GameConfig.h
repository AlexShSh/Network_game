#pragma once

#include <SFML/Graphics.hpp>

namespace conf
{

enum
{
    ConDelay = 50,
    RestartWaiting = 100
};

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

    const sf::String hp = "HP:";
    const char filename1[] = "images/walker1.png";
    const char filename2[] = "images/walker3.png";
    const float speed = 0.2;
    const float animation_speed = 0.04;
    const int frame_amount = 16;
    const int health = 1000;
    const int shooting_delay = 500;
    const int sprite_width = 64;
    const int sprite_height = 64;
    const int obj_width = 20;
    const int obj_height = 25;
    const int text_indent_x = -20;
    const int text_indent_y = -60;
    const float collide_speed = 0.01;

    const char font_filename[] = "images/CyrilicOld.TTF";
}

namespace Bullet
{
    const char filename[] = "images/FireBall_new.png";
    const float speed = 0.4;
    const float animation_speed = 0.01;
    const int frame_amount = 8;
    const int sprite_width = 64;
    const int sprite_height = 64;
    const int obj_width = 20;
    const int obj_height = 20;
    const float default_pos_x = 1500;
    const float default_pos_y = 1000;
}

namespace Enemy
{
    const char filename[] = "images/Devil3.0.png";
    const float speed = 0.1;
    const float animation_speed = 0.01;
    const int frame_amount = 8;
    const int sprite_width = 64;
    const int sprite_height = 64;
    const int obj_width = 25;
    const int obj_height = 30;
    const int health = 1;
    const float default_pos_x = 1500;
    const float default_pos_y = 1000;
    const float collide_speed = 0.02;
}


enum ObjectType
{
    EMPTY,
    PLAYER,
    ENEMY,
    BULLET,
};

namespace Window
{
    const int width = 640;
    const int height = 512;
    const char window_name[] = "Clash of Robots";
}

namespace Map
{
    const int width = 1120;
    const int height = 800;
    const char filename[] = "images/map1.png";
    const int frame_height = 25;
    const int frame_width = 35;

    const int sprite_width = 32;
    const int sprite_height = 32;
    const sf::String TileMap[conf::Map::frame_height] = {
            "00000000000000000000000000000000000",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "0                                 0",
            "00000000000000000000000000000000000",
    };

    const int border_width = 32;
}

}