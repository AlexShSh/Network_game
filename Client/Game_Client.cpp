#include "Game_Client.h"

Game_Client::Game_Client() :
    client(),
    game(),
    keyboard_thread(&Game_Client::keyboard_read, this)
{}


bool Game_Client::play()
{
    if (!client.find_server())
        return false;

    if (!client.connect())
        return false;

    sf::Packet packet;

    game.start();
    keyboard_thread.launch();

    while (true)
    {
        if (!game.update_window())
            break;

        if (!client.recieve(packet))
            break;

        game.update_objects(packet);
        game.render();

        auto input = game.get_input();
        sf::Packet pack;
        pack << (sf::Int16) net::Data << (sf::Int16) input.dir << (sf::Int16) input.is_shoot;
        client.send(pack);
    }
    game.set_active(false);
    return true;
}

void Game_Client::keyboard_read()
{
    game.keyboard_reader();
}