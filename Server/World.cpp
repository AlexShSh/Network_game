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
    for (auto it = objects.begin(); it != objects.end(); )
    {
        GameObject* obj = *it;

        obj->update(time, objects);

        if (obj->get_type() == conf::ObjectType::PLAYER)
        {
            auto player = dynamic_cast<Player*> (obj);
            if (player->is_shoot())
            {
                make_shoot(player);
            }
        }

        if (!obj->get_active() && obj->get_type() == conf::ObjectType::BULLET)
        {
            auto bul = dynamic_cast<Bullet*> (obj);
            it = objects.erase(it);
            disactive_bullets.emplace_back(bul);
        }
        else
            it++;
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
    auto bul = get_bullet(player->get_position(), player->get_direction(), player);

    objects.emplace_back(bul);

    player->set_shoot_ready(false);
}

Bullet* World::get_bullet(sf::Vector2f pos, conf::Dir dir_, Player* creator)
{
    if (!disactive_bullets.empty())
    {
        Bullet* bul = disactive_bullets.back();
        disactive_bullets.pop_back();
        bul->set_position(pos);
        bul->set_direction(dir_);
        bul->set_creator(creator);
        bul->set_active(true);

        return bul;
    }
    else
        return new Bullet(pos.x, pos.y, dir_, creator);
}

World::~World()
{
    for (auto obj : objects)
    {
        delete obj;
    }
    objects.clear();

    for (auto bul : disactive_bullets)
    {
        delete bul;
    }
    disactive_bullets.clear();
}

int World::disact_players_num()
{
    int count = 0;
    for (auto pl : players)
    {
        if (!pl.second->get_active())
            count++;
    }
    return count;
}