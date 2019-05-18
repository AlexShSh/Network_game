#include "Client.h"
#include <X11/Xlib.h>
/*
int main()
{
    XInitThreads();
    try
    {
        std::cout << "Enter server ip: ";
        std::string str;
        std::cin >> str;

        sf::IpAddress server_ip(str);

        Client client(server_ip, net::ServerPort);
        Game game;

        client.start(&game);
    }
    catch (std::runtime_error& err)
    {
        std::cout << err.what() << std::endl;
    }

    return 0;
}
*/
//sf::IpAddress::LocalHost

int main()
{
    Client client;
    if (!client.find_server())
        return -1;

    client.connect();
}