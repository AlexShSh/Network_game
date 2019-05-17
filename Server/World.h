#pragma once

#include <map>
#include <list>

#include "GameObject.h"
#include "Player.h"
#include "Bullet.h"
#include "../Shared/NetworkDefinitions.h"
#include "ClientHandler.h"


class World
{
public:
    World() = default;
    ~World();
    void create_players(std::list<ClientHandler>& clients);
    bool upd_players_from_packs(std::list<ClientHandler>& clients);
    void update_objects(sf::Time time);
    sf::Packet create_game_state();
    void delete_disconnected(std::list<ClientId>& disconnected);
    void make_shoot(Player* player);
    int disact_players_num();

private:
    std::map<ClientId, Player*>  players;
    std::list<GameObject*> objects;
    std::list<Bullet*> disactive_bullets;

    Bullet* get_bullet(sf::Vector2f pos, conf::Dir dir_, Player* creator);
};
