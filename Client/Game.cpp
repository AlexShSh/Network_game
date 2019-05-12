#include "Game.h"

Game::Game() :
    is_active(false)
{}

void Game::update_players(sf::Packet& packet, float time)
{
    ClientId id;
    float x, y;
    sf::Int16 dir_tmp;
    while(packet >> id >> x >> y >> dir_tmp)
    {
        //std::cout << id << " " << x << " " << y << " " << dir_tmp << std::endl;
        Dir dir = (Dir) dir_tmp;
        if (dir == NONE && players.count(id) != 0)
            players.erase(id);

        if (players.count(id) == 0)
            players.emplace(id, GraphObject("hero.png", 96, 96, x, y, dir, 0.005, 3));

        players[id].set_position(x, y, dir);
        //players[id].animation(dir, time);
    }
    //std::cout << std::endl;
}

void Game::start()
{
    window = new sf::RenderWindow(sf::VideoMode(600, 500), "Stannis Baratheon");
    window->clear();
    window->display();

    is_active = true;

    std::cout << "Game start" << std::endl;
}

void Game::keyboard_reader()
{
    sf::Event ev;
    ev.type = sf::Event::GainedFocus;
    while (is_active && window->waitEvent(ev))
    {
        sf::sleep(sf::milliseconds(50));
        Dir dir = keyboard.get_direction();
        if (dir != NONE)
        {
            packet.clear();
            packet << (sf::Int16) dir;
        }
    }
}

sf::Packet Game::get_packet()
{
    sf::Packet copy = packet;
    packet.clear();

    return copy;
}

void Game::render()
{
    window->clear();
    for (auto& pl : players)
    {
        pl.second.draw(window);
    }
    window->display();
}