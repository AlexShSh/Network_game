#pragma once

#include <map>
#include <vector>
#include <sstream>

#include "GraphObject.h"
#include "../Shared/NetworkDefinitions.h"
#include "KeyboardInterface.h"


class Game
{
public:
    Game();
    ~Game();
    void start();
    void update_objects(sf::Packet& packet);
    void update_player(sf::Packet& packet);
    void update_bullet(sf::Packet& packet, int counter);
    void update_ememy(sf::Packet& packet, int counter);
    void keyboard_reader();
    PlayerInput get_input();
    void render();
    void set_active(bool b);
    void map_render(sf::RenderWindow* window);

    bool update_window();


private:
    std::map<ClientId, GraphObject> players;
    std::vector<GraphObject> bullets;
    std::vector<GraphObject> enemies;
    KeyboardInterface keyboard;
    sf::RenderWindow* window;
    PlayerInput inp;
    bool is_active;
    bool window_focused;
    std::ostringstream player_hp;
    std::map<ClientId, sf::Text> hp;
    sf::Font cyrilic;

    sf::Texture robot1;
    sf::Texture robot2;
    sf::Texture bullet;
    sf::Texture enemy;
    sf::Texture map;
    GraphObject Map;

    sf::Mutex mutex;

};
