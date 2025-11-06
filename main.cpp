#include "server/Server.hpp"
#include "server/Authentication.hpp"
#include "server/Client.hpp"
#include <iostream>
#include <sys/socket.h>

int ServerFd;

int main(int ac,char **av)
{
    if (ac != 3)
    {   
        std::cerr << "./ircserv <port> <password>" << "\n";
        return 1;    
    }
    ServerFd = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerFd)
    {

    }
}
