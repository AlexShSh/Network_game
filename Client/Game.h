#pragma once

#include <map>
#include <vector>

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
    void keyboard_reader();
    sf::Packet get_packet();
    void render();
    void set_active(bool b);

    bool update_window();


private:
    std::map<ClientId, GraphObject> players;
    std::vector<GraphObject> bullets;
    KeyboardInterface keyboard;
    sf::RenderWindow* window;
    sf::Packet packet;
    bool is_active;
    bool window_focused;
    sf::Texture robot;
    sf::Texture bullet;


};
