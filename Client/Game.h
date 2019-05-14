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
    void update_players(sf::Packet& packet, float time);
    void keyboard_reader();
    sf::Packet get_packet();
    void render();
    void set_active(bool b);

private:
    std::map<ClientId, GraphObject> players;
    KeyboardInterface keyboard;
    sf::RenderWindow* window;
    sf::Packet packet;
    bool is_active;
};
