#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <vector>

#include "../Shared/NetworkDefinitions.h"
#include "World.h"
#include "ClientHandler.h"

class Server
{
public:
    Server();


    void start();
    //bool start(World* world);
    //void start_listen_connection();

    bool is_active();

    ~Server();

private:
    sf::IpAddress ipAddress;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    sf::UdpSocket con_socket;

    sf::Int32 max_clients;
    sf::Int32 cur_clients;

    std::list<ClientHandler> clients;
    std::list<ClientId> disconnected;

    ClientId last_id;

    sf::Thread listen_con_thread;
    sf::Thread cmd_line_thread;
    sf::Thread connect_clients_thread;

    sf::Mutex mutex;
    bool active;


    void recive();
    void listen_connection();
    void cmd_line_read();
    bool add_client();
    bool send_id(sf::TcpSocket* socket, ClientId id);
    bool send_serv_full(sf::TcpSocket* socket);
    bool broadcast(sf::Packet& packet);
    ClientId get_id();
    void add_disconnected_packet(sf::Packet& packet);
    void set_active(bool atc);
    void connect_clients();
};
