#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet(float x, float y, conf::Dir dir);

    void update(sf::Time time, std::list<GameObject*>& objects) override;
    void compress_packet(sf::Packet& packet) override;
    void interract(std::list<GameObject*>& objects) override;
};
