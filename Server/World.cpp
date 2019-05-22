#include "World.h"

#include <iostream>


void World::create_players(std::list<ClientId> clients)
{
    for (auto cl : clients)
    {
        float x = 0, y = 0;
        if (cl % 2)
        {
            x = 100 * (1 + (cl / 2));
            y = 100;
        }
        else
        {
            x = conf::Map::width - 100 * (cl / 2);
            y = conf::Map::height - 100;
        }

        auto new_pl = new Player(x, y, conf::Dir::LEFT, cl);
        players.emplace(cl, new_pl);
        objects.emplace_back(new_pl);
    }
}

bool World::upd_players_from_packs(std::map<ClientId, ClientHandler*>* clients)
{
    if (clients->empty())
        return false;

    for (auto& cl : *clients)
    {
        players[cl.second->get_id()]->open_packet(cl.second->get_rcv_packet());
    }
    return true;
}

void World::update_objects(sf::Time time)
{
    for (auto it = objects.begin(); it != objects.end();) {
        GameObject *obj = *it;

        obj->update(time, objects);

        if (obj->get_type() == conf::ObjectType::PLAYER) {
            auto player = dynamic_cast<Player *> (obj);
            if (player->is_shoot()) {
                make_shoot(player);
            }
        }
        if(obj->get_type() == conf::ObjectType::ENEMY && !obj->get_active())
        {
            it = objects.erase(it);
            enemies--;
            continue;
        }
        if (!obj->get_active() && obj->get_type() == conf::ObjectType::BULLET) {
            auto bul = dynamic_cast<Bullet *> (obj);
            it = objects.erase(it);
            disactive_bullets.emplace_back(bul);
        } else
            it++;
    }
}

sf::Packet World::create_game_state() {
    sf::Packet packet;
    for (auto &obj : objects) {
        obj->compress_packet(packet);
    }

    return packet;
}

void World::delete_disconnected(std::list<ClientId> disconnected)
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

void World::make_shoot(Player *player) {
    auto bul = get_bullet(player->get_position(), player->get_direction(), player);

    objects.emplace_back(bul);

    player->set_shoot_ready(false);
}

Bullet *World::get_bullet(sf::Vector2f pos, conf::Dir dir_, Player *creator) {
    if (!disactive_bullets.empty()) {
        Bullet *bul = disactive_bullets.back();
        disactive_bullets.pop_back();
        bul->set_position(pos);
        bul->set_direction(dir_);
        bul->set_creator(creator);
        bul->set_active(true);

        return bul;
    } else
        return new Bullet(pos.x, pos.y, dir_, creator);
}

World::~World() {
    for (auto obj : objects) {
        delete obj;
    }
    objects.clear();

    for (auto bul : disactive_bullets) {
        delete bul;
    }
    disactive_bullets.clear();

}

int World::disact_players_num() {
    int count = 0;
    for (auto pl : players) {
        if (!pl.second->get_active())
            count++;
    }
    return count;
}

void World::generator(sf::Time time)
{
    if(time.asSeconds()/20 > wave)
    {
        wave++;
        counter = 0;
    }

    if(counter != wave * 3 && (time.asSeconds() - 20 * (wave - 1)) > (int)(4 / wave + 1)  * counter && enemies < 30)
    {
        auto en = new Enemy(512, 512, conf::Dir::RIGHT, counter++);
        enemies++;
        objects.emplace_back(en);

        if(wave > 5)
        {
            auto en1 = new Enemy(1000, 500, conf::Dir::RIGHT, counter++);
            enemies++;
            objects.emplace_back(en1);
        }
    }
}

World::World()
{
    enemies = 0;
}