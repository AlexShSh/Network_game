#include "Game.h"

Game::Game() :
    is_active(false)
{}

void Game::update_players(sf::Packet& packet)
{
    ClientId id;
    float x, y;
    sf::Int16 dir_tmp;

    packet >> id >> x >> y >> dir_tmp;

        Dir dir = (Dir) dir_tmp;
        if (dir == NONE)
        {
            players.erase(id);
            return;
        }

        if (players.count(id) == 0)
            players.emplace(id, GraphObject("hero.png", 96, 96, x, y, dir, 0.005, 3));
        players[id].frame_pos(dir);
        players[id].set_position(x, y, dir);

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
    while (is_active)
    {
        if (window_focused)
        {
            Dir dir = keyboard.get_direction();
            if (dir != NONE)
            {
                packet.clear();
                packet << (sf::Int16) dir;
            }
            sf::sleep(sf::milliseconds(20));
        }

    }
}

sf::Packet Game::get_packet()
{
    sf::Packet copy = packet;
    packet.clear();

    return copy;
}

void Game::window_clear()
{
    window->clear();
}

void Game::render()
{
    for (auto& pl : players)
    {
        pl.second.draw(window);
    }
    window->display();
}

Game::~Game()
{
    window->close();
}

void Game::set_active(bool b)
{
    is_active = b;
}

bool Game::update_window()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::GainedFocus:
                window_focused = true;
                break;
            case sf::Event::LostFocus:
                window_focused = false;
                break;
            case sf::Event::Closed:
                set_active(false);
                window->close();
                return false;
            default:
                break;
        }
    }
    return true;
}