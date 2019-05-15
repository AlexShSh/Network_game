#include "World.h"

#include <iostream>


void World::create_players(std::list<ClientHandler> &clients)
{
    for (auto& cl : clients)
    {
        ClientId id = cl.get_id();
        auto new_pl = new Player(100 * id, 100 * id, LEFT);
        players.emplace(id, new_pl);
        objects.emplace_back(new_pl);
    }
}

bool World::update_players(std::list<ClientHandler> &clients, sf::Time time)
{
    if (clients.empty())
        return false;

    for (auto& cl : clients)
    {
        players[cl.get_id()]->update_from_packet(cl.get_rcv_packet(), time);
    }
    return true;
}

sf::Packet World::create_game_state()
{
    sf::Packet packet;
    for (auto& pl : players)
    {
        packet << pl.first << pl.second->get_position().x <<
        pl.second->get_position().y << (sf::Int16) pl.second->get_direction() << pl.second->get_current_frame();
    }

    return packet;
}

void World::delete_disconnected(std::list<ClientId> &disconnected)
{
    for (auto& id : disconnected)
    {
        auto pl = players[id];
        delete pl;
        players.erase(id);
    }
}