#include "Game.h"

Game::Game() :
    is_active(false),
    window_focused(false)
{}

void Game::update_players(sf::Packet& packet)
{
    sf::Int16 type_tmp;
    ClientId id;
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;

    while (packet >> type_tmp >> id >> x >> y >> dir_tmp >> current_frame)
    {
        auto type = (conf::ObjectType) type_tmp;
        if (type != conf::ObjectType::PLAYER)
            continue;

        auto dir = (conf::Dir) dir_tmp;
        if (dir == conf::Dir::NONE)
        {
            players.erase(id);
            return;
        }
        //std::cout << x << " " << y << std::endl;
        if (players.count(id) == 0)

            players.emplace(id, GraphObject(&lion, 64, 64, 250, 250, conf::Dir::LEFT));


        players[id].frame_pos(dir, current_frame);
        players[id].set_position(x, y, dir);
    }
}

void Game::start()
{
    window = new sf::RenderWindow(sf::VideoMode(conf::Map::width, conf::Map::height), "Stannis Baratheon");
    window->clear();
    window->display();
    lion.loadFromFile("images/walker1.png");

    is_active = true;

    std::cout << "Game start" << std::endl;
}

void Game::keyboard_reader()
{
    while (is_active)
    {
        if (window_focused)
        {
            conf::Dir dir = keyboard.get_direction();
            bool is_shoot = keyboard.get_shoot();

            packet.clear();
            if (dir != conf::Dir::NONE || is_shoot)
            {
                packet << (sf::Int16) dir << (sf::Int16) is_shoot;
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


void Game::render()
{
    window->clear();
    for (auto& pl : players)
    {
        pl.second.draw(window);
    }
    window->display();
}

Game::~Game()
{
    window->close();
    delete window;
}

void Game::set_active(bool b)
{
    is_active = b;
}

bool Game::update_window()
{
    sf::Event event = {};
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