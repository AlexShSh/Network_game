#include "Client.h"
#include <X11/Xlib.h>

int main()
{
    Client client(sf::IpAddress::LocalHost, Network::ServerPort);
    XInitThreads();
    Game game;

    client.start(&game);

    return 0;
}