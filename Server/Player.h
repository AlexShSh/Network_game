#pragma once

#include "GameObject.h"
#include "../Shared/NetworkDefinitions.h"

class Player : public GameObject
{
public:
    Player(float x, float y, conf::Dir dir_, ClientId id_);

    void open_packet(sf::Packet packet) override;
    void compress_packet(sf::Packet& packet) override;
    void update(sf::Time time) override;

private:
    int health;
    bool live;
    ClientId id;
    conf::Dir moving_dir;
};
