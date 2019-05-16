#include "Game.h"

Game::Game() :
    is_active(false),
    window_focused(false)
{}

void Game::update_objects(sf::Packet& packet)
{
    sf::Int16 type_tmp;
    ClientId id;
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;
    int counter = 0;

    while (packet >> type_tmp >> id >> x >> y >> dir_tmp >> current_frame)
    {
        auto type = (conf::ObjectType) type_tmp;
        auto dir = (conf::Dir) dir_tmp;

        if (type == conf::ObjectType::PLAYER)
            update_player(id, x, y, dir, current_frame);

        else if(type == conf::ObjectType::BULLET)
        {

            if(counter == bullets.size())
                bullets.emplace_back(GraphObject(&bullet, conf::Bullet::sprite_width, conf::Bullet::sprite_height,
                                                 1000, 1000, conf::Dir::LEFT));
            update_bullet(x, y, dir, current_frame, counter++);
        }
        else
            continue;
    }

    while(counter < bullets.size())
        bullets[counter++].set_position(1000, 1000, conf::LEFT);
}


void Game::update_player(ClientId id, float x, float y, conf::Dir dir, int current_frame)
{
    if (dir == conf::Dir::NONE)
    {
        players.erase(id);
        return;
    }

    if (players.count(id) == 0)
        players.emplace(id, GraphObject(&robot, conf::Player::sprite_width, conf::Player::sprite_height,
                                        250, 250, conf::Dir::LEFT));


    players[id].frame_pos(dir, current_frame);
    players[id].set_position(x, y, dir);
}

void Game::update_bullet(float x, float y, conf::Dir dir, int current_frame, int counter)
{
    bullets[counter].frame_pos(dir, current_frame);
    bullets[counter].set_position(x, y, dir);
}

void Game::start()
{
    window = new sf::RenderWindow(sf::VideoMode(conf::Map::width, conf::Map::height), "Stannis Baratheon");
    window->clear();
    window->display();
    robot.loadFromFile(conf::Player::filename);
    bullet.loadFromFile(conf::Bullet::filename);

    for(int i = 0; i < 10; i++)
        bullets.emplace_back(GraphObject(&bullet, conf::Bullet::sprite_width, conf::Bullet::sprite_height,
                                     1000, 1000, conf::Dir::LEFT));


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
    for(auto& bul : bullets)
    {
        bul.draw(window);
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