#include "Server.h"

Server::Server() :
    ipAddress(sf::IpAddress::getLocalAddress()),
    port(Network::ServerPort),
    max_players(Network::MaxPlayersNum),
    cur_players(0),
    con_delay(Network::ConnectionDelay),
    last_id(0)
{
    listener.listen(port);
    selector.add(listener);

    std::cout << "Server start at: " << ipAddress << ":" << port << std::endl;
}

bool Server::connect_clients()
{
    std::cout << "Wait to connect " << max_players << " players" << std::endl;
    while (true)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
                add_client();
            if (cur_players == max_players)
            {
                std::cout << "All players connected" << std::endl;
                return true;
            }

        }
    }
    return false;
}

bool Server::add_client()
{
    auto new_socket = new sf::TcpSocket;
    new_socket->setBlocking(false);

    if (listener.accept(*new_socket) == sf::Socket::Done)
    {
        selector.add(*new_socket);
        ClientId id = get_id();
        if (!send_id(new_socket, id))
        {
            delete new_socket;
            return false;
        }
        clients.emplace_back(new_socket, id);
        cur_players++;

        std::cout << "New player connected, current player's number: " << cur_players << std::endl;
        return true;
    }

    delete new_socket;
    return false;
}

void Server::recive()
{
    if (selector.wait(sf::Time::Zero))
    {
        for (auto it = clients.begin(); it != clients.end();)
        {
            ClientHandler& client = *it;
            auto sock = client.get_socket();

            if (selector.isReady(*sock))
            {
                sf::Packet pack;
                auto status = sock->receive(pack);
                if (status == sf::Socket::Done)
                {
                    client.set_packet(pack);
                    while (sock->receive(pack) == sf::Socket::Done)
                        pack.clear();

                    it++;
                }
                else if (status == sf::Socket::Disconnected ||
                         status == sf::Socket::Error)
                {
                    selector.remove(*sock);
                    it = clients.erase(it);

                    disconnected.emplace_back(client.get_id());
                    cur_players--;

                    std::cout << "Client was disconected\n";
                }
            }
            else
            {
                it++;
            }
        }
    }
}

ClientId Server::get_id()
{
    last_id++;
    return last_id;
}

bool Server::send_id(sf::TcpSocket *socket, ClientId id)
{
    sf::Packet id_pack;
    id_pack << id;
    if (socket->send(id_pack) != sf::Socket::Done)
    {
        std::cout << "Couldn't send id" << std::endl;
        return false;
    }

    return true;
}

bool Server::broadcast(sf::Packet &packet)
{
    for (auto& cl : clients)
    {
        auto sock = cl.get_socket();
        auto status = sock->send(packet);

        while (status == sf::Socket::Partial)
            status = sock->send(packet);

        if (status == sf::Socket::Done)
            continue;
        else
        {
            std::cout << "Couldn't send packet" << std::endl;
            return false;
        }
    }
    return true;
}

Server::~Server()
{
    std::cout << "Server was destroyed" << std::endl;
}

bool Server::start(World *world)
{
    connect_clients();

    std::cout << clients.size() << std::endl;

    world->create_players(clients);

    sf::Clock timer;

    while (true)
    {
        if (timer.getElapsedTime().asMilliseconds() >= con_delay)
        {
            if (!world->update_players(clients, timer.restart()))
                break;

            sf::Packet pack = world->create_game_state();
            if (!disconnected.empty())
            {
                add_disconnected_packet(pack);
                disconnected.clear();
            }

            broadcast(pack);

            recive();
            if (!disconnected.empty())
            {
                world->delete_disconnected(disconnected);
            }
        }
    }
}


void Server::add_disconnected_packet(sf::Packet &packet)
{
    for (auto cl : disconnected)
    {
        packet << cl << -1.f << -1.f << (sf::Int16) NONE;
    }
}