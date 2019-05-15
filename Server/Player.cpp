#include "Player.h"

Player::Player(float x, float y, Dir dir_) :
    GameObject(x, y, dir_, _animation_speed, _frame_amount, PLAYER),
    health(player_health),
    live(true)
{}