#include "Client.h"
#include <X11/Xlib.h>

int main()
{
    XInitThreads();
    try
    {
        Client client(sf::IpAddress::LocalHost, Network::ServerPort);
        Game game;

        client.start(&game);
    }
    catch (std::runtime_error& err)
    {
        std::cout << err.what() << std::endl;
    }

    return 0;
}

