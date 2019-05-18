#include "Server.h"
/*
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

 */


void command_line_reader(Server* serv)
{
    while (serv->is_active())
    {
        std::string str;
        std::getline(std::cin, str);
        if (str == "q")
        {
            serv->set_active(false);
        }
    }
}

int main()
{
    Server server;

    server.connect_clients();
}