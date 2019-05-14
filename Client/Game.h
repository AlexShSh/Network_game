#pragma once

#include <map>

#include "GraphObject.h"
#include "../Shared/NetworkDefinitions.h"
#include "KeyboardInterface.h"


class Game
{
public:
    Game();
    ~Game();
    void start();
    void update_players(sf::Packet& packet);
    void keyboard_reader();
    sf::Packet get_packet();
    void render();
    void set_active(bool b);
    bool update_window();

private:
    std::map<ClientId, GraphObject> players;
    KeyboardInterface keyboard;
    sf::RenderWindow* window;
    sf::Packet packet;
    bool is_active;
    bool window_focused;
};
