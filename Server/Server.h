#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include <map>

#include "../Shared/NetworkDefinitions.h"
#include "World.h"
#include "ClientHandler.h"

class Server
{
public:
    explicit Server(int max_clients_ = net::MaxClientsNum);
    void start();
    bool broadcast(sf::Packet& packet);
    void recive();
    std::list<ClientId> get_new_clients();
    std::list<ClientId> get_disconnected_clients();
    std::map<ClientId, ClientHandler*>* get_clients_ptr();
    bool is_active();
    bool empty();
    void stop();

    ~Server();

private:
    sf::IpAddress ipAddress;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    sf::UdpSocket con_socket;

    sf::Int32 max_clients;
    sf::Int32 cur_clients;

    std::map<ClientId, ClientHandler*> clients;
    std::map<ClientId, sf::Clock> temp_disconnected;
    std::list<ClientId > new_clients;
    std::list<ClientId> comp_disconnected;

    ClientId last_id;

    sf::Thread listen_con_thread;
    sf::Thread cmd_line_thread;
    sf::Thread connect_clients_thread;

    sf::Mutex mutex;
    bool active;


    void listen_connection();
    void cmd_line_read();
    bool add_client();
    bool send_id(sf::TcpSocket* socket, ClientId id);
    bool send_serv_full(sf::TcpSocket* socket);
    void set_active(bool atc);
    void connect_clients();
    void check_temp_disconnected();
    ClientId get_id();
};
