#pragma once

#include <SFML/Network.hpp>

using PortNumber = unsigned short;
using ClientId   = sf::Int16 ;

namespace Network
{

enum
{
    ServerPort = 5678,
    ConnectionDelay = 30,
    MaxPlayersNum = 1
};

}