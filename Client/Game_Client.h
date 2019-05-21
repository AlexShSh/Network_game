#pragma once

#include "Client.h"
#include "Game.h"


class Game_Client
{
public:
    Game_Client();
    bool play();

private:
    Client client;
    Game game;
    sf::Thread keyboard_thread;

    void keyboard_read();
};

