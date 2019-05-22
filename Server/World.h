#pragma once

#include <map>
#include <list>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "../Shared/NetworkDefinitions.h"
#include "ClientHandler.h"
#include "Enemy.h"


class World
{
public:
    World();
    ~World();
    void create_players(std::list<ClientId > clients);
    bool upd_players_from_packs(std::map<ClientId, ClientHandler*>* clients);
    void update_objects(sf::Time time);
    sf::Packet create_game_state();
    void delete_disconnected(std::list<ClientId> disconnected);
    void generator(sf::Time time);

private:
    std::map<ClientId, Player*>  players;
    std::list<GameObject*> objects;
    std::list<Bullet*> disactive_bullets;
    int enemies;
    int counter;
    int wave;

    Bullet* get_bullet(sf::Vector2f pos, conf::Dir dir_, Player* creator);
    void make_shoot(Player* player);
};
