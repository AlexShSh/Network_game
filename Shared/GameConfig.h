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

enum {PLAYER_SPEED = 10};

sf::Packet& operator << (sf::Packet& packet, Dir dir)
{
    return packet << ((sf::Int16) dir);
}

sf::Packet& operator >> (sf::Packet& packet, Dir& dir)
{
    sf::Int8 num;
    packet >> num;
    dir = (Dir) num;
    return packet;
}
