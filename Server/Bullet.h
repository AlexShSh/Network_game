#pragma once

#include "GameObject.h"
#include "Player.h"

class Bullet : public GameObject
{
public:
    Bullet(float x, float y, conf::Dir dir, Player* creator_);

    void update(sf::Time time, std::list<GameObject*>& objects) override;
    void compress_packet(sf::Packet& packet) override;

    Player* get_creator() const;
    void set_creator(Player* pl);


private:
    Player* creator;

    void interract(std::list<GameObject*>& objects) override;
};
