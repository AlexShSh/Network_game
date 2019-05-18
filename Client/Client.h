#pragma once

#include <SFML/Network.hpp>
#include <iostream>

#include "Game.h"


class Client
{
public:
    //Client(sf::IpAddress serv_ip, PortNumber serv_port);
    Client();
    bool find_server();
    bool connect();
    bool send(sf::Packet& packet);
    bool recieve(sf::Packet& packet);

    //bool start(Game* game);

    ~Client();

private:
    sf::TcpSocket socket;
    sf::UdpSocket con_socket;

    sf::IpAddress server_ip;

    ClientId id;

    bool recive_id();
    bool reconect();
};
