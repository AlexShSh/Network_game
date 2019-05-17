#include "Game.h"

Game::Game() :
    is_active(false),
    window_focused(false)
{}

void Game::update_objects(sf::Packet& packet)
{
    sf::Int16 type_tmp;
    int counter = 0;

    while (packet >> type_tmp)
    {
        auto type = (conf::ObjectType) type_tmp;


        if (type == conf::ObjectType::PLAYER)
            update_player(packet);

        else if(type == conf::ObjectType::BULLET)
        {
            if(counter == bullets.size())
                bullets.emplace_back(GraphObject(&bullet, conf::Bullet::sprite_width, conf::Bullet::sprite_height,
                        conf::Bullet::default_pos_x, conf::Bullet::default_pos_y, conf::Dir::LEFT));
            update_bullet(packet, counter++);
        }
        else if(type == conf::ObjectType::ENEMY)
            update_ememy(packet);
        else
            continue;
    }

    while(counter < bullets.size())
        bullets[counter++].set_position(conf::Bullet::default_pos_x, conf::Bullet::default_pos_y, conf::LEFT);
}


void Game::update_player(sf::Packet& packet)
{
    ClientId id;
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;
    int health;

    packet >> id >> x >> y >> dir_tmp >> current_frame >> health;

    auto dir = (conf::Dir) dir_tmp;

    if (dir == conf::Dir::NONE)
    {
        players.erase(id);
        hp.erase(hp.begin() + id - 1);
        return;
    }

    if (players.count(id) == 0)
    {
    sf::Texture* tx = (id % 2 ? &robot1 : &robot2);

    players.emplace(id, GraphObject(tx, conf::Player::sprite_width, conf::Player::sprite_height,
                                    250, 250, conf::Dir::LEFT));
        hp.emplace_back(sf::Text("", cyrilic, 20));
        hp.back().setFillColor(sf::Color::Red);

    }

    players[id].frame_pos(dir, current_frame);
    players[id].set_position(x, y, dir);

    player_hp << health;
    hp[id - 1].setString(conf::Player::hp + player_hp.str());
    hp[id - 1].setPosition(x + conf::Player::text_indent_x, y + conf::Player::text_indent_y);
    player_hp.str(std::string());
}

void Game::update_ememy(sf::Packet &packet)
{
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;
    int number;

    packet >> number >> x >> y >> dir_tmp >> current_frame;

    auto dir = (conf::Dir) dir_tmp;

    if (dir == conf::Dir::NONE)
    {
        enemies.erase(enemies.begin() + number);
        return;
    }

    if (number + 1 >  enemies.size())
    {
        enemies.emplace_back(GraphObject(&enemy, conf::Enemy::sprite_width, conf::Enemy::sprite_height,
                                        250, 250, conf::Dir::LEFT));
    }

    enemies[number].frame_pos(dir, current_frame);
    enemies[number].set_position(x, y, dir);
}

void Game::update_bullet(sf::Packet& packet, int counter)
{
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;

    packet >> x >> y >> dir_tmp >> current_frame;
    auto dir = (conf::Dir) dir_tmp;

    bullets[counter].frame_pos(dir, current_frame);
    bullets[counter].set_position(x, y, dir);
}


void Game::start()
{
    window = new sf::RenderWindow(sf::VideoMode(conf::Map::width, conf::Map::height), conf::Map::window_name);
    window->clear();
    window->display();

    robot1.loadFromFile(conf::Player::filename1);
    robot2.loadFromFile(conf::Player::filename2);
    bullet.loadFromFile(conf::Bullet::filename);
    enemy.loadFromFile(conf::Enemy::filename);
    map.loadFromFile(conf::Map::filename);

    Map = GraphObject(&map, conf::Map::sprite_width, conf::Map::sprite_height, 0, 0, conf::DOWN);

    cyrilic.loadFromFile(conf::Player::font_filename);

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
    map_render(window);
    for (auto& pl : players)
        pl.second.draw(window);

    for(auto& bul : bullets)
        bul.draw(window);

    for(auto& text : hp)
        window->draw(text);

    for(auto& en : enemies)
        en.draw(window);

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

void Game::map_render(sf::RenderWindow* window) {
    for (int y = 0; y < conf::Map::frame_height; y++)
        for (int x = 0; x < conf::Map::frame_width; x++)
        {
            if(conf::Map::TileMap[y][x] == ' ')
                Map.frame_pos(conf::DOWN, 0);
            if(conf::Map::TileMap[y][x] == '0')
                Map.frame_pos(conf::DOWN, 1);
            Map.set_position((x + 0.5) * conf::Map::sprite_width, (y + 0.5) * conf::Map::sprite_height, conf::NONE);
            Map.draw(window);
        }
}