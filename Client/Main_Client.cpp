#include "Game_Client.h"
#include <X11/Xlib.h>

int main()
{
    XInitThreads();

    Game_Client game_client;

    game_client.play();


    return 0;
}

