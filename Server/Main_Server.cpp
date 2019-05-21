#include "Game_Server.h"
#include <X11/Xlib.h>


int main()
{
    XInitThreads();

   /* Server server;
    server.connect_clients();

    while (true)
    {
        World world;

        bool status = server.start(&world);

        if (!status)
            break;
    }*/

    Game_Server game_server;
    game_server.play();


    return 0;
}



/*
int main()
{
    Server server;

    server.start();
    while (server.is_active())
    {
        server.recive();
    }

    return 0;
}*/