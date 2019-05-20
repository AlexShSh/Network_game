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
    void update_ememy(sf::Packet& packet);
    void keyboard_reader();
    sf::Packet get_packet();
    void render();
    void set_active(bool b);
    void map_render(sf::RenderWindow* window);

    std::ostringstream player_hp;
    std::vector<sf::Text> hp;
    sf::Font cyrilic;

    bool update_window();


private:
    std::map<ClientId, GraphObject> players;
    std::vector<GraphObject> bullets;
    std::vector<GraphObject*> enemies;
    KeyboardInterface keyboard;
    sf::RenderWindow* window;
    sf::Packet packet;
    bool is_active;
    bool window_focused;

    sf::Texture robot1;
    sf::Texture robot2;
    sf::Texture bullet;
    sf::Texture enemy;
    sf::Texture map;
    GraphObject Map;


};
