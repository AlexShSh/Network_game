#include "Client.h"
#include <X11/Xlib.h>

int main()
{
    XInitThreads();

    Client client(sf::IpAddress::LocalHost, Network::ServerPort);
    Game game;

    client.start(&game);

    return 0;
}