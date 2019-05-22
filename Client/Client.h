#pragma once

#include <SFML/Network.hpp>
#include <iostream>

#include "Game.h"


class Client
{
public:
    Client();
    bool find_server();
    bool connect();
    ClientId get_id();

    bool send(sf::Packet& packet);
    bool recieve(sf::Packet& packet);
    void disconnect();

    ~Client();

private:
    sf::TcpSocket socket;
    sf::UdpSocket con_socket;

    sf::IpAddress server_ip;

    ClientId id;

    bool recive_id();
    bool reconect();
};
