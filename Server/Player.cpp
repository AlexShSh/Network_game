#include "Player.h"

Player::Player(float x, float y, conf::Dir dir_) :
    GameObject(x, y, dir_, conf::animation_speed, conf::frame_amount, conf::PLAYER),
    health(conf::player_health),
    live(true)
{}