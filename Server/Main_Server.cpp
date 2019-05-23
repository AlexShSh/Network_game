#include "Game_Server.h"
#include <X11/Xlib.h>


int main()
{
    XInitThreads();

    Game_Server game_server;
    game_server.play();

    return 0;
}



