#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <vector>

#include "NetworkDefinitions.h"
#include "World.h"
#include "ClientHandler.h"

class Server
{
public:
    Server();

    bool connect_clients();
    bool start(World* world);
    ~Server();

private:
    sf::IpAddress ipAddress;
    PortNumber port;
    sf::TcpListener listener;
    sf::SocketSelector selector;

    sf::Int32 max_players;
    sf::Int32 cur_players;
    sf::Int32 con_delay;

    std::list<ClientHandler> clients;

    ClientId last_id;

    void recive();
    bool add_client();
    bool send_id(sf::TcpSocket* socket, ClientId id);
    bool broadcast(sf::Packet& packet);
    ClientId get_id();

};


