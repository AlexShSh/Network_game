#include "Server.h"

Server::Server(int max_clients_) :
    ipAddress(sf::IpAddress::getLocalAddress()),
    max_clients(max_clients_),
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

        sf::Packet packet;
        sf::Clock timer;
        sf::Socket::Status status = {};

        while (timer.getElapsedTime().asMilliseconds() < net::Timeout)
        {
            status = new_socket->receive(packet);
            if (status == sf::Socket::Done)
                break;
        }

        if (status != sf::Socket::Done)
        {
            selector.remove(*new_socket);
            new_socket->disconnect();
            delete new_socket;
            return false;
        }

        sf::Int16 type_tmp;
        packet >> type_tmp;
        auto type = (net::PacketType) type_tmp;

        if (type == net::PacketType::NewConnect)
        {
            if (cur_clients == max_clients)
            {
                send_serv_full(new_socket);
                selector.remove(*new_socket);
                new_socket->disconnect();
                delete new_socket;
                return false;
            }

            ClientId id = get_id();
            if (!send_id(new_socket, id))
            {
                selector.remove(*new_socket);
                new_socket->disconnect();
                delete new_socket;
                return false;
            }

            sf::Lock lock(mutex);
            auto  handler = new ClientHandler(new_socket, id);
            clients.emplace(id, handler);
            cur_clients++;
            new_clients.emplace_back(id);

            std::cout << "New client connected, current client's number: " << cur_clients << std::endl;
            return true;
        }
        else if (type == net::PacketType::Reconnect)
        {
            ClientId id;
            packet >> id;

            sf::Lock lock(mutex);

            if (clients.count(id) == 0 || temp_disconnected.count(id) == 0)
            {
                selector.remove(*new_socket);
                new_socket->disconnect();
                delete new_socket;
                return false;
            }

            ClientHandler* cl = clients[id];
            cl->change_socket(new_socket);
            temp_disconnected.erase(id);

            std::cout << "Client " << id << " reconnected" << std::endl;
            return true;
        }

        selector.remove(*new_socket);
        new_socket->disconnect();
    }

    delete new_socket;
    return false;
}

void Server::recive()
{
    if (selector.wait(sf::milliseconds(net::Timeout)))
    {
        sf::Lock lock(mutex);
        for (auto el : clients)
        {
            auto client = el.second;
            auto sock = client->get_socket();

            if (selector.isReady(*sock))
            {
                sf::Packet pack;
                auto status = sock->receive(pack);
                if (status == sf::Socket::Done)
                {
                    sf::Int16 type_tmp;
                    pack >> type_tmp;
                    auto type = (net::PacketType) type_tmp;

                    if (type == net::PacketType::Data)
                    {
                        client->set_packet(pack);
                        while (sock->receive(pack) == sf::Socket::Done)
                            pack.clear();
                    }
                    else if (type == net::PacketType::Disconnect)
                    {
                        auto id = client->get_id();

                        selector.remove(*sock);
                        comp_disconnected.emplace_back(id);
                        cur_clients--;
                        delete clients[id];
                        clients.erase(id);

                        std::cout << "Client " << id << " was disconected\n";
                    }
                }
                else if (status == sf::Socket::Disconnected)
                {
                    temp_disconnected.emplace(client->get_id(), sf::Clock());
                }
            }
        }
    }
    check_temp_disconnected();
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
    sf::Lock lock(mutex);
    for (auto& cl : clients)
    {
        auto sock = cl.second->get_socket();
        auto status = sock->send(packet);

        while (status == sf::Socket::Partial)
            status = sock->send(packet);

        if (status == sf::Socket::Done)
            continue;
        else if (status != sf::Socket::Disconnected)
        {
            std::cout << "Couldn't send packet" << std::endl;
            return false;
        }
    }
    return true;
}

Server::~Server()
{
    set_active(false);
    std::cout << "Server was destroyed" << std::endl;
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
        PortNumber remote_port = 0;
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
    set_active(true);
    listen_con_thread.launch();
    connect_clients_thread.launch();
    cmd_line_thread.launch();
}

void Server::check_temp_disconnected()
{
    for (auto it = temp_disconnected.begin(); it != temp_disconnected.end();)
    {
        auto cl = *it;
        if (cl.second.getElapsedTime().asMilliseconds() >= net::ReconnectTimeout)
        {
            auto hd = clients[cl.first];
            auto sock = hd->get_socket();
            auto id = cl.first;

            selector.remove(*sock);
            comp_disconnected.push_back(id);
            cur_clients--;
            delete clients[id];
            clients.erase(id);
            it = temp_disconnected.erase(it);

            std::cout << "Client " << id << " was disconected\n";
        }
        else
        {
            it++;
        }
    }
}

std::list<ClientId> Server::get_new_clients()
{
    sf::Lock lock(mutex);
    auto cpy = new_clients;
    new_clients.clear();
    return cpy;
}

std::list<ClientId> Server::get_disconnected_clients()
{
    sf::Lock lock(mutex);
    auto cpy = comp_disconnected;
    comp_disconnected.clear();
    return cpy;
}

bool Server::empty()
{
    sf::Lock lock(mutex);
    return clients.empty();
}

std::map<ClientId, ClientHandler*>* Server::get_clients_ptr()
{
    return &clients;
}

void Server::stop()
{
    set_active(false);
}