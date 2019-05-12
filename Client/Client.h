#pragma once

#include <SFML/Network.hpp>
#include <iostream>

#include "../Shared/NetworkDefinitions.h"
#include "Game.h"

class Client
{
public:
    Client(sf::IpAddress serv_ip, PortNumber serv_port);
    bool send(sf::Packet& packet);
    bool recieve(sf::Packet& packet);
    bool recive_id();

    bool start(Game* game);

    ~Client();

private:
    sf::TcpSocket socket;

    sf::IpAddress server_ip;
    PortNumber server_port;

    sf::IpAddress local_ip;
    PortNumber local_port;

    ClientId id;
};
