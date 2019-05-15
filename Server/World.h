#pragma once

#include <map>
#include <list>

#include "GameObject.h"
#include "Player.h"
#include "../Shared/NetworkDefinitions.h"
#include "ClientHandler.h"


class World
{
public:
    World() = default;
    void create_players(std::list<ClientHandler>& clients);
    bool upd_players_from_packs(std::list<ClientHandler>& clients);
    void update_objects(sf::Time time);
    sf::Packet create_game_state();
    void delete_disconnected(std::list<ClientId>& disconnected);

private:
    std::map<ClientId, Player*>  players;
    std::list<GameObject*> objects;
};
