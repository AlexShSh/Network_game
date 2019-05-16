#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet(float x, float y, conf::Dir dir);

    void update(sf::Time time) override;
    void compress_packet(sf::Packet& packet) override;
};
