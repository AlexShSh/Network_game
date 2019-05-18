#pragma once

#include <SFML/Network.hpp>

using PortNumber = unsigned short;
using ClientId   = sf::Int16 ;

namespace net
{

enum
{
    ServerPort = 25678,
    ServerConnectPort = 30957,
    ConnectionDelay = 30,
    ConnectAttempt = 5,
    ConnectionTimeout = 500,
    Timeout = 50,
    RestartWaiting = 100,
    MaxPlayersNum = 3
};

enum PacketType
{
    ConRequest,
    ConConfirm,
    ServerFull,
    SendID
};

}