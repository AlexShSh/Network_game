#pragma once

#include <SFML/Network.hpp>

#include "../Shared/NetworkDefinitions.h"


class ClientHandler
{
public:
    ClientHandler(sf::TcpSocket* sock, ClientId id_);
    ClientHandler(const ClientHandler& hndl);
    void set_packet(sf::Packet packet);
    void change_socket(sf::TcpSocket* new_socket);
    sf::TcpSocket* get_socket() const;
    ClientId get_id() const;
    sf::Packet get_rcv_packet() const;
    ~ClientHandler();

private:
    sf::TcpSocket* socket;
    ClientId id;
    sf::Packet rcv_pack;
};
