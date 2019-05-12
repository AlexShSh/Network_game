#include "Server.h"

int main()
{
    Server server;

    World world;

    server.start(&world);

    return 0;
}