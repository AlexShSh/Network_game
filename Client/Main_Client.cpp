#include "Client.h"

int main()
{
    Client client(sf::IpAddress::LocalHost, Network::ServerPort);

    Game game;

    client.start(&game);

    return 0;
}