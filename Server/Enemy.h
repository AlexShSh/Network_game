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

    void get_damage(int size) override;

private:
    int number;
    int health;
    conf::Dir moving_dir;
    bool can_move;
    bool death;
    int generation_count;
    conf::Dir old_dir;

    void interract(std::list<GameObject *>& objects, sf::Time time) override;
};
