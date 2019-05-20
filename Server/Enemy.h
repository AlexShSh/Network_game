#pragma once

#include "GameObject.h"
#include "../Shared/NetworkDefinitions.h"

class Enemy : public GameObject
{
public:
    Enemy(float x, float y, conf::Dir dir, int number);

    void compress_packet(sf::Packet& packet) override;
    void update(sf::Time time, std::list<GameObject*>& objects) override;

    conf::Dir get_direction(std::list<GameObject *>& objects);

    int get_number() const;

    void get_damage();

private:
    int number;
    int health;
    conf::Dir moving_dir;
    bool can_move;
    bool death;

    void interract(std::list<GameObject *>& objects) override;
};