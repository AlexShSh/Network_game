#include "Server.h"

Server::Server() :
    ipAddress(sf::IpAddress::getLocalAddress()),
    port(Network::ServerPort),
    max_players(Network::MaxPlayersNum),
    cur_players(0),
    con_delay(Network::ConnectionDelay)
{
    listener.listen(port);
    selector.add(listener);

    recived.reserve(max_players);

    std::cout << "Server start at: " << ipAddress << ":" << port << std::endl;
}

bool Server::connect_clients()
{
    while (true)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
                add_client();
            if (cur_players == max_players)
                return true;
        }
    }
}

bool Server::add_client()
{
    auto new_socket = new sf::TcpSocket;
    new_socket.setBlocking(false);

    if (listener.accept(*new_socket) == sf::Socket::Done)
    {
        selector.add(*new_socket);
        clients.emplace_back(new_socket);
        cur_players++;

        std::cout << "New player connected, current player's number: " << cur_players << std::endl;
        return true;
    }

    delete new_socket;
    return false;
}

void Server::recive()
{
    recived.clear();

    if (selector.wait(sf::Time::Zero))
    {

    }
}