#pragma once

#include "GameObject.h"
#include "../Shared/NetworkDefinitions.h"

class Player : public GameObject
{
public:
    Player(float x, float y, conf::Dir dir_, ClientId id_);
    void open_packet(sf::Packet packet);

    void compress_packet(sf::Packet& packet) override;
    void update(sf::Time time) override;

    bool is_shoot() const;
    ClientId get_id() const;
    void set_shoot_ready(bool st);

private:
    int health;
    bool live;
    ClientId id;
    conf::Dir moving_dir;
    bool shoot_click;
    bool shoot_ready;
    sf::Time time_after_shoot;
};
