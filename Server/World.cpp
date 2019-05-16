#include "World.h"

#include <iostream>


void World::create_players(std::list<ClientHandler> &clients)
{
    for (auto& cl : clients)
    {
        ClientId id = cl.get_id();

        auto new_pl = new Player(100 * id, 100 * id, conf::Dir::LEFT, id);
        players.emplace(id, new_pl);
        objects.emplace_back(new_pl);

    }
}

bool World::upd_players_from_packs(std::list<ClientHandler> &clients)
{
    if (clients.empty())
        return false;

    for (auto& cl : clients)
    {
        players[cl.get_id()]->open_packet(cl.get_rcv_packet());
    }
    return true;
}

void World::update_objects(sf::Time time)
{
    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        auto obj = *it;

        obj->update(time);

        if (obj->get_type() == conf::ObjectType::PLAYER)
        {
            auto player = dynamic_cast<Player*> (obj);
            if (player->is_shoot())
            {
                make_shoot(player);
            }
        }

        /*if (!obj->get_active())
        {
            delete obj;
            objects.erase(it);
        }*/
    }
}

sf::Packet World::create_game_state()
{
    sf::Packet packet;
    for (auto& obj : objects)
    {
        obj->compress_packet(packet);
    }

    return packet;
}

void World::delete_disconnected(std::list<ClientId> &disconnected)
{
    for (auto& id : disconnected)
    {
        for (auto it = objects.begin(); it != objects.end(); it++)
        {
            if (*it == players[id])
            {
                delete players[id];
                players.erase(id);
                objects.erase(it);
                break;
            }
        }
    }
}

void World::make_shoot(Player* player)
{
    auto bul = new Bullet(player->get_position().x, player->get_position().y,
                          player->get_direction());

    objects.emplace_back(bul);

    std::cout << "Player " << player->get_id() << " shoot\n";
    player->set_shoot_ready(false);
}