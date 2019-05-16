#include "Server.h"

int main()
{
    Server server;
    server.connect_clients();

    while (true)
    {
        World world;

        bool status = server.start(&world);

        if (!status)
            break;
    }

    return 0;
}