#pragma once

#include <SFML/Network.hpp>

using PortNumber = unsigned short;
using ClientId   = sf::Int16 ;

namespace net
{

enum
{

    ServerPort = 22418,
    ServerConnectPort = 32157,
    ConnectAttempt = 5,
    ConnectionTimeout = 500,
    ReconnectTimeout = 250,
    ReconnectAttemp = 5,
    ReconnectDelay = 30,
    Timeout = 50,
    NoID = 0,
    MaxClientsNum = 2
};

enum PacketType
{
    ConRequest,
    ConConfirm,
    ServerFull,
    SendID,
    NewConnect,
    Reconnect,
    Disconnect,
    Data
};

}