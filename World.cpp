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

void World::update_players(std::list<ClientHandler> &clients, sf::Time time)
{
    for (auto& cl : clients)
    {
        players[cl.get_id()].update_from_packet(cl.get_rcv_packet(), time);
    }
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