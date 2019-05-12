#include "World.h"

World::World()
{}

void World::create_players(std::list<ClientHandler> &clients)
{
    for (auto& cl : clients)
    {
        players.emplace(cl.get_id(), GameObject());
    }
}

bool World::update_players(std::list<ClientHandler> &clients, sf::Time time)
{
    if (clients.empty())
        return false;

    for (auto& cl : clients)
    {
        players[cl.get_id()].update_from_packet(cl.get_rcv_packet(), time);
    }
    return true;
}

sf::Packet World::create_game_state()
{
    sf::Packet packet;
    for (auto& pl : players)
    {
        packet << pl.first << pl.second.get_position().x <<
        pl.second.get_position().y << pl.second.get_direction();
    }

    return packet;
}

void World::delete_disconnected(std::list<ClientId> &disconnected)
{
    for (auto& id : disconnected)
    {
        players.erase(id);
    }
}