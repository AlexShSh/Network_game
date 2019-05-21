#include "Game_Server.h"

Game_Server::Game_Server() :
    server()
{
    world = new World;
    players = server.get_clients_ptr();
}

Game_Server::Game_Server(int max_players) :
    world(),
    server(max_players)
{
    players = server.get_clients_ptr();
}


bool Game_Server::play()
{
    server.start();

    while (server.empty());

    sf::Clock timer;
    world->wave = 0;
    sf::Clock begin_time;
    std::list<ClientId> disc = {};

    while (server.is_active())
    {
        if (timer.getElapsedTime().asMilliseconds() >= conf::ConDelay)
        {
            world->create_players(server.get_new_clients());

            if (!world->upd_players_from_packs(players))
            {
                delete world;
                while (server.empty());
                world = new World;
                continue;
            }

            world->generator(begin_time.getElapsedTime());
            world->update_objects(timer.restart());

            sf::Packet pack = world->create_game_state();


            add_disconnected(pack, disc);
            disc.clear();

            server.broadcast(pack);

            server.recive();

            disc = server.get_disconnected_clients();
            world->delete_disconnected(disc);

            /*if (world.disact_players_num() >= players->size() - 1 && players->size() != 1)
            {
                restart_counter++;
                if (restart_counter >= conf::RestartWaiting)
                    return true;
            }*/
        }
    }
    server.stop();
    return false;
}

void Game_Server::add_disconnected(sf::Packet &pack, std::list<ClientId> disc)
{
    //std::list<ClientId> disc = server.get_disconnected_clients();
    for (auto cl : disc)
    {
        std::cout << "!!!0000!!!!\n";
        pack << (sf::Int16) conf::ObjectType::PLAYER << cl << -1.f << -1.f << (sf::Int16) conf::Dir::NONE << -1.f;
    }
}
