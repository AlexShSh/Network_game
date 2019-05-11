#pragma once

#include <map>
#include "list"

#include "GameObject.h"
#include "NetworkDefinitions.h"
#include "ClientHandler.h"

class World
{
public:
    World();
    void create_players(std::list<ClientHandler>& clients);
    void update_players(std::list<ClientHandler>& clients, sf::Time time);
    sf::Packet create_game_state();

private:
    std::map<ClientId, GameObject>  players;
};