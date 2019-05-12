#include "Game.h"

Game::Game() :
    is_active(false)
{}

void Game::update_players(sf::Packet& packet)
{
    ClientId id;
    float x, y;
    Dir dir;
    while(!packet.endOfPacket())
    {
        packet >> id >> x >> y >> dir;

        if (dir == NONE && players.count(id) != 0)
            players.erase(id);

        if (players.count(id) == 0)
        {
            GraphObject new_pl("hero.png", 96, 96, x, y, dir, 0.005, 3);
            players.emplace(id, new_pl);
        } else
        {
            players[id].set_position(x, y, dir);
        }
    }
}

void Game::start()
{
    window = new sf::RenderWindow(sf::VideoMode(600, 500), "Stannis Baratheon");
    window->clear();
    window->display();

    is_active = true;
}

void Game::keyboard_reader()
{
    while (is_active)
    {
        sf::sleep(sf::milliseconds(50));
        Dir dir = keyboard.get_direction();
        if (dir != NONE)
        {
            packet.clear();
            packet << dir;
        }
    }
}

sf::Packet Game::get_packet()
{
    return packet;
}