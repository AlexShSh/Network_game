#include "Game.h"

Game::Game() :
    is_active(false),
    window_focused(false)
{}

void Game::update_objects(sf::Packet& packet)
{
    sf::Int16 type_tmp;
    int counter_bull = 0, counter_dev = 0;

    while (packet >> type_tmp)
    {
        auto type = (conf::ObjectType) type_tmp;


        if (type == conf::ObjectType::PLAYER)
            update_player(packet);

        else if (type == conf::ObjectType::BULLET)
        {
            if(counter_bull == bullets.size())

                bullets.emplace_back(GraphObject(&bullet, conf::Bullet::sprite_width, conf::Bullet::sprite_height,
                        conf::Bullet::default_pos_x, conf::Bullet::default_pos_y, conf::Dir::LEFT));
            update_bullet(packet, counter_bull++);
        }
        else if(type == conf::ObjectType::ENEMY)
        {
            if(counter_dev == enemies.size())
                enemies.emplace_back(GraphObject(&enemy, conf::Enemy::sprite_width, conf::Enemy::sprite_height,
                        conf::Enemy::default_pos_x, conf::Enemy::default_pos_y, conf::Dir::LEFT));
            update_ememy(packet, counter_dev++);
        }
        else
            continue;
    }

    while(counter_bull < bullets.size())
        bullets[counter_bull++].set_position(conf::Bullet::default_pos_x, conf::Bullet::default_pos_y, conf::LEFT);

    while(counter_dev < enemies.size())
        enemies[counter_dev++].set_position(conf::Enemy::default_pos_x, conf::Enemy::default_pos_y, conf::LEFT);
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
        hp.erase(id);
        return;
    }

    if (players.count(id) == 0)
    {
        sf::Texture* tx = (id % 2 ? &robot1 : &robot2);

        players.emplace(id, GraphObject(tx, conf::Player::sprite_width, conf::Player::sprite_height,
                                    250, 250, conf::Dir::LEFT));
        hp.emplace(id, sf::Text("", cyrilic, 20));
        hp[id].setFillColor(sf::Color::Red);

    }

    players[id].frame_pos(dir, current_frame);
    players[id].set_position(x, y, dir);

    if(id == owner)
        set_camera(x, y);

    player_hp << health;
    hp[id].setString(conf::Player::hp + player_hp.str());
    hp[id].setPosition(x + conf::Player::text_indent_x, y + conf::Player::text_indent_y);
    player_hp.str(std::string());
}

void Game::update_ememy(sf::Packet &packet, int counter)
{
    float x, y;
    int current_frame;
    sf::Int16 dir_tmp;
    int number;

    packet >> number >> x >> y >> dir_tmp >> current_frame;

    auto dir = (conf::Dir) dir_tmp;

    enemies[counter].frame_pos(dir, current_frame);
    enemies[counter].set_position(x, y, dir);
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
    window = new sf::RenderWindow(sf::VideoMode(conf::Window::width, conf::Window::height), conf::Window::window_name);
    camera.reset(sf::FloatRect(0, 0 ,conf::Window::width, conf::Window::height));
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

            mutex.lock();
            inp.dir = dir;
            inp.is_shoot = is_shoot;
            mutex.unlock();

            sf::sleep(sf::milliseconds(20));
        }
    }
}

PlayerInput Game::get_input()
{
    sf::Lock lock(mutex);

    return inp;
}


void Game::render()
{
    window->setView(camera);
    window->clear();
    map_render(window);
    for (auto& pl : players)
        pl.second.draw(window);

    for(auto& bul : bullets)
        bul.draw(window);

    for(auto& text : hp)
        window->draw(text.second);

    for(auto& en : enemies)
    {
        en.draw(window);
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

void Game::map_render(sf::RenderWindow* window) {
    for (int y = 0; y < conf::Map::frame_height; y++)
        for (int x = 0; x < conf::Map::frame_width; x++)
        {
            if(conf::Map::TileMap[y][x] == ' ')
                Map.frame_pos(conf::DOWN, 0);
            if(conf::Map::TileMap[y][x] == '0')
                Map.frame_pos(conf::DOWN, 1);
            Map.set_position((x + 0.5f) * conf::Map::sprite_width, (y + 0.5f) * conf::Map::sprite_height, conf::NONE);
            Map.draw(window);
        }
}

void Game::set_owner(ClientId id)
{
    owner = id;
}

void Game::set_camera(float x, float y)
{
    float cam_x = x;
    float cam_y = y;

    float win_x = conf::Window::width;
    float win_y = conf::Window::height;
    float map_x = conf::Map::width;
    float map_y = conf::Map::height;

    if(cam_x < win_x / 2)
        cam_x = win_x / 2;

    if(cam_y < win_y / 2)
        cam_y = win_y / 2;

    if(cam_x > map_x - win_x / 2)
        cam_x = map_x - win_x / 2;
    if(cam_y > map_y - win_y / 2)
        cam_y = map_y - win_y / 2;
    camera.setCenter(cam_x, cam_y);

}