#include <iostream>
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "Player.h"

Player::Player(std::string file, int width, int height, float x, float y, float animation_speed, int frame_amount) :
GraphObj(file, width, height, x, y, animation_speed, frame_amount)
{}

int Player::get_direction()
{
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        return RIGHT;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
        return UP;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
        return DOWN;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
        return LEFT;
    return -1;
}

void Player::update(float time)
{
    float dx = 0, dy = 0;
    switch(get_direction()) {
        case RIGHT: {
            animation(RIGHT, time);
            dx = speed * time;
            direction = RIGHT;

            break;
        }
        case UP: {
            animation(UP, time);
            dy = -speed * time;
            direction = UP;
            break;
        }
        case DOWN: {
            animation(DOWN, time);
            dy = speed * time;
            direction = DOWN;
            break;
        }
        case LEFT: {
            animation(LEFT, time);
            dx = -speed * time;
            direction = LEFT;
            break;
        }
        default:
            frame_pos(direction);
    }
        move(dx,dy);
}