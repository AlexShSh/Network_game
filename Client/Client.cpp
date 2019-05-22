#include "Client.h"

Client::Client() :
    id(net::NoID)
{}

bool Client::find_server()
{
    con_socket.setBlocking(false);
    con_socket.bind(sf::Socket::AnyPort);

    sf::Packet packet;
    packet << (sf::Int16) net::PacketType::ConRequest;

    for (int i = 0; i < net::ConnectAttempt; i++)
    {
        auto status = con_socket.send(packet, sf::IpAddress::Broadcast, net::ServerConnectPort);
        if (status != sf::Socket::Done)
        {
            continue;
        }
        sf::Clock timer;
        while (timer.getElapsedTime().asMilliseconds() < net::ConnectionTimeout)
        {
            sf::Packet rcv_pack;
            sf::IpAddress remote_ip;
            PortNumber remote_port;
            status = con_socket.receive(rcv_pack, remote_ip, remote_port);

            if (status != sf::Socket::Done)
                continue;

            sf::Int16 type_tmp;
            if (!(rcv_pack >> type_tmp))
                continue;

            auto type = (net::PacketType) type_tmp;
            if (type == net::PacketType::ConConfirm)
            {
                server_ip = remote_ip;
                std::cout << "Get confirm from " << remote_ip << ":" << remote_port << std::endl;
                return true;
            }
        }
    }
    std::cout << "Time is out, server wasn't found" << std::endl;
    return false;
}

bool Client::send(sf::Packet &packet)
{
    sf::Packet send_packet = packet;
    //send_packet << (sf::Int16) net::Data << packet;

    auto status = socket.send(send_packet);
    if (status == sf::Socket::Done)
    {
        return true;
    } 
    else if (status == sf::Socket::Disconnected)
    {
        return reconect();
    }
    
    std::cout << "Can't send packet" << std::endl;
    return false;
}

bool Client::recieve(sf::Packet &packet)
{
    sf::Packet rcv_pack;
    socket.setBlocking(true);

    sf::Clock timer;
    sf::Socket::Status status = {};
    while (timer.getElapsedTime().asMilliseconds() < net::Timeout)
    {
        status = socket.receive(rcv_pack);
        if (status == sf::Socket::Status::Done)
            break;
    }

    if (status == sf::Socket::Disconnected)
    {
        return reconect();
    }
    else if (status != sf::Socket::Done)
        std::cout << "Can't recive packet" << std::endl;

    //sf::Int16 type_tmp;
    //rcv_pack >> type_tmp;
    //auto type = (net::PacketType) type_tmp;
    //if (type == net::PacketType::Data)
    {
        packet.clear();
        packet = rcv_pack;
    }
    return true;
}

bool Client::recive_id()
{
    socket.setBlocking(false);
    sf::Packet packet;
    sf::Clock timer;

    while (socket.receive(packet) != sf::Socket::Done)
    {
        if (timer.getElapsedTime().asMilliseconds() >= net::Timeout)
        {
            std::cout << "Can't recieve id" << std::endl;
            return false;
        }
    }

    sf::Int16 type_tmp;
    if (!(packet >> type_tmp))
    {
        std::cout << "Unknown packet" << std::endl;
        return false;
    }

    auto type = (net::PacketType) type_tmp;
    if (type == net::PacketType::SendID)
    {
        packet >> id;
        std::cout << "ID: " << id << std::endl;
        return true;
    }
    else if (type == net::PacketType::ServerFull)
    {
        std::cout << "Server is full" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Unknown type of packet" << std::endl;
        return false;
    }
}
/*
void keyboard_reader(Game* game)
{
    game->keyboard_reader();
}
 */
/*
bool Client::start(Game* game)
{
    if (!recive_id())
        return false;

    sf::Packet packet;

    game->start();

    sf::Thread keyboard_thread(keyboard_reader, game);
    keyboard_thread.launch();

    while (true)
    {
        if (!game->update_window())
            break;

        if (!recieve(packet))
            break;

        game->update_objects(packet);
        game->render();

        sf::Packet send_packet = game->get_input();
        send(send_packet);
    }
    game->set_active(false);
    return true;
}
*/

Client::~Client()
{
    std::cout << "Client was destroyed" << std::endl;
}

bool Client::connect()
{
    socket.setBlocking(true);
    auto status = socket.connect(server_ip, net::ServerPort, sf::milliseconds(net::Timeout));

    if (status != sf::Socket::Done)
    {
        std::cout << "Can't connect to server" << std::endl;
        return false;
    }

    sf::Packet packet;
    packet << (sf::Int16) net::PacketType::NewConnect;

    auto stat = socket.send(packet);
    if (stat != sf::Socket::Done)
    {
        std::cout << "Can't send 'New connect' packet" << std::endl;
        return false;
    }

    return recive_id();
}

bool Client::reconect()
{
    for (int i = 0; i < net::ReconnectAttemp; ++i)
    {
        sf::sleep(sf::milliseconds(net::ReconnectDelay));

        socket.setBlocking(true);
        auto status = socket.connect(server_ip, net::ServerPort, sf::milliseconds(net::Timeout));

        if (status != sf::Socket::Done)
        {
            std::cout << "Can't connect to server" << std::endl;
            continue;
        }

        sf::Packet packet;
        packet << (sf::Int16) net::PacketType::Reconnect << id;

        auto stat = socket.send(packet);
        if (stat != sf::Socket::Done)
        {
            std::cout << "Can't send 'Reconnect' packet" << std::endl;
            continue;
        }
        return true;
    }

    return false;
}

void Client::disconnect()
{
    sf::Packet packet;
    packet << (sf::Int16) net::Disconnect;

    send(packet);

    socket.disconnect();
    return;
}

ClientId Client::get_id()
{
    return id;
}