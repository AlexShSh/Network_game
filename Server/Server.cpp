#include "Server.h"

Server::Server() :
    ipAddress(sf::IpAddress::getLocalAddress()),
    max_clients(net::MaxPlayersNum),
    cur_clients(0),
    last_id(0),
    listen_con_thread(&Server::listen_connection, this),
    cmd_line_thread(&Server::cmd_line_read, this),
    connect_clients_thread(&Server::connect_clients, this)
{
    listener.listen(net::ServerPort);
    selector.add(listener);

    std::cout << "Server start at: " << ipAddress << ":" << net::ServerPort << std::endl;
}

void Server::connect_clients()
{
    while (is_active())
    {
        if (selector.wait(sf::milliseconds(net::Timeout)))
        {
            if (selector.isReady(listener))
                    add_client();
        }
    }
}

bool Server::add_client()
{
    auto new_socket = new sf::TcpSocket;
    new_socket->setBlocking(false);

    if (listener.accept(*new_socket) == sf::Socket::Done)
    {
        selector.add(*new_socket);

        if (cur_clients == max_clients)
        {
            send_serv_full(new_socket);
            new_socket->disconnect();
            selector.remove(*new_socket);
            delete new_socket;
            return false;
        }

        ClientId id = get_id();
        if (!send_id(new_socket, id))
        {
            new_socket->disconnect();
            selector.remove(*new_socket);
            delete new_socket;
            return false;
        }
        clients.emplace_back(new_socket, id);
        cur_clients++;

        std::cout << "New player connected, current player's number: " << cur_clients << std::endl;
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
                    disconnected.emplace_back(client.get_id());
                    cur_clients--;
                    it = clients.erase(it);

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
    id_pack << (sf::Int16) net::PacketType::SendID << id;
    if (socket->send(id_pack) != sf::Socket::Done)
    {
        std::cout << "Couldn't send id" << std::endl;
        return false;
    }

    return true;
}

bool Server::send_serv_full(sf::TcpSocket *socket)
{
    sf::Packet pack;
    pack << (sf::Int16) net::PacketType::ServerFull;
    if (socket->send(pack) != sf::Socket::Done)
    {
        std::cout << "Couldn't send 'server ful'" << std::endl;
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

/*
bool Server::start(World *world)
{
    world->create_players(clients);

    sf::Clock timer;
    int restart_counter = 0;

    while (true)
    {
        if (timer.getElapsedTime().asMilliseconds() >= con_delay)
        {
            if (!world->upd_players_from_packs(clients))
            {
                return false;
            }

            world->update_objects(timer.restart());

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
            if (world->disact_players_num() >= clients.size() - 1 && clients.size() != 1)
            {
                restart_counter++;
                if (restart_counter >= net::RestartWaiting)
                    return true;
            }
        }
    }
}
*/

void Server::add_disconnected_packet(sf::Packet &packet)
{
    for (auto cl : disconnected)
    {
        packet << (sf::Int16) conf::ObjectType::PLAYER << cl << -1.f << -1.f << (sf::Int16) conf::Dir::NONE << -1.f;
    }
}


void Server::listen_connection()
{
    sf::IpAddress local_ip = sf::IpAddress::getLocalAddress();
    auto status = con_socket.bind(net::ServerConnectPort);
    if (status != sf::Socket::Done)
    {
        std::cout << "Can't bind connection socket" << std::endl;
        return;
    }
    std::cout << "Start listen on " << local_ip << ":" << net::ServerConnectPort << std::endl;

    con_socket.setBlocking(false);

    while (is_active())
    {
        sf::IpAddress remote_ip;
        PortNumber remote_port;
        sf::Packet rcv_pack;

        status = sf::Socket::Disconnected;

        while (is_active() && status != sf::Socket::Done)
            status = con_socket.receive(rcv_pack, remote_ip, remote_port);

        sf::Int16 type_tmp;
        if (!(rcv_pack >> type_tmp))
            continue;

        auto type = (net::PacketType) type_tmp;
        if (type == net::PacketType::ConRequest)
        {
            std::cout << "Connection request from: " << remote_ip << ":" << remote_port << std::endl;
            sf::Packet packet;
            packet << (sf::Int16) net::PacketType::ConConfirm;
            status = con_socket.send(packet, remote_ip, remote_port);
            if (status != sf::Socket::Done)
            {
                std::cout << "Can't send connection confirm" << std::endl;
            }
        }
    }
    return;
}

bool Server::is_active()
{
    sf::Lock lock(mutex);

    return active;
}

void Server::set_active(bool act)
{
    sf::Lock lock(mutex);
    active = act;
}

void Server::cmd_line_read()
{
    while (is_active())
    {
        std::string str;
        std::getline(std::cin, str);
        if (str == "q")
        {
            set_active(false);
        }
    }
}

void Server::start()
{
    listen_con_thread.launch();
    connect_clients_thread.launch();
    cmd_line_thread.launch();


}