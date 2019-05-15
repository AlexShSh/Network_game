#include "Client.h"


Client::Client(sf::IpAddress serv_ip, PortNumber serv_port) :
    server_ip(serv_ip),
    server_port(serv_port),
    id(-1)
{
    local_ip = sf::IpAddress::getLocalAddress();

    auto status = socket.connect(server_ip, server_port);

    if (status != sf::Socket::Done)
    {
        std::cout << "Can't connect to server" << std::endl;
        throw std::runtime_error("Can't connect to server");
    }


    local_port = socket.getLocalPort();
}

bool Client::send(sf::Packet &packet)
{
    if (socket.send(packet) != sf::Socket::Done)
    {
        std::cout << "Can't send packet" << std::endl;
        return false;
    }

    return true;
}

bool Client::recieve(sf::Packet &packet)
{
    packet.clear();
    auto status = socket.receive(packet);

    if (status == sf::Socket::Disconnected)
    {
        std::cout << "Server was disconected" << std::endl;
        return false;
    }
    else if (status != sf::Socket::Done)
        std::cout << "Can't recive packet" << std::endl;

    return true;
}

bool Client::recive_id()
{
    sf::Packet packet;
    if (socket.receive(packet) != sf::Socket::Done)
    {
        std::cout << "Can't recieve id" << std::endl;
        return false;
    }

    if (!(packet >> id))
    {
        std::cout << "Unexpected packet" << std::endl;
        return false;
    }

    std::cout << "ID: " << id << std::endl;

    return true;
}

void keyboard_reader(Game* game)
{
    game->keyboard_reader();
}

bool Client::start(Game* game)
{
    if (!recive_id())
        return false;

    sf::Packet packet;

    game->start();

    sf::Thread keyboard_thread(keyboard_reader, game);
    keyboard_thread.launch();

    sf::Clock timer;

    while (true)
    {
        if (!game->update_window())
            break;

        if (!recieve(packet))
            break;

        game->update_players(packet, timer.restart().asMilliseconds());
        game->render();

        sf::Packet send_packet = game->get_packet();
        send(send_packet);
    }
    game->set_active(false);
    return true;
}

Client::~Client()
{
    std::cout << "Client was destroyed" << std::endl;
}