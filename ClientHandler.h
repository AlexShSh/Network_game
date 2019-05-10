#pragma once

#include <SFML/Network.hpp>

#include "NetworkDefinitions.h"

class ClientInfo
{
public:
    ClientInfo(sf::TcpSocket* sock);
    ClientInfo(sf::IpAddress ip, PortNumber port_);
    std::pair<sf::IpAddress, PortNumber> get_info() const;

private:
    sf::IpAddress ipAddress;
    PortNumber port;
};


class ClientHandler
{
public:
    ClientHandler(sf::TcpSocket* sock);
    sf::TcpSocket* get_socket() const;
    std::pair<sf::IpAddress, PortNumber> get_info() const;
    ~ClientHandler();

private:
    sf::TcpSocket socket;
    ClientInfo info;
};
