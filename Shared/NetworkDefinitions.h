#pragma once

#include <SFML/Network.hpp>

using PortNumber = unsigned short;
using ClientId   = sf::Int8;

namespace Network
{

enum
{
    ServerPort = 5678,
    ConnectionDelay = 20,
    MaxPlayersNum = 2
};

}