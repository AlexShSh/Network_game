#include "ClientHandler.h"

ClientHandler::ClientHandler(sf::TcpSocket *sock, ClientId id_) :
    socket(sock),
    id(id_)
{}

ClientHandler::ClientHandler(const ClientHandler &hndl)
{
    id = hndl.id;
    socket = hndl.socket;
}

sf::TcpSocket* ClientHandler::get_socket() const
{
    return socket;
}

ClientId ClientHandler::get_id() const
{
    return id;
}

sf::Packet ClientHandler::get_rcv_packet() const
{
    return rcv_pack;
}

ClientHandler::~ClientHandler()
{
    delete socket;
}

void ClientHandler::set_packet(sf::Packet packet)
{
    rcv_pack = packet;
}

void ClientHandler::change_socket(sf::TcpSocket *new_socket)
{
    delete socket;
    socket = new_socket;
}