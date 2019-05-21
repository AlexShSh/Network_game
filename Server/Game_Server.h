#pragma once

#include "Server.h"
#include "World.h"


class Game_Server
{
public:
    Game_Server();
    explicit Game_Server(int max_players);
    bool play();

private:
    World* world;
    Server server;
    std::map<ClientId, ClientHandler*>* players;

    void add_disconnected(sf::Packet& pack, std::list<ClientId> disc);
};
