#include "server/Server.hpp"
#include <csignal>
#include <exception>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

void    close_all_fds(void)
{
    int max_fd = OPEN_MAX;
    int i = 3;

    while (i < max_fd)
    {
        close(i);
        i++;
    }
}

int main(int ac,char **av)
{
    
    if (ac != 3 )
    {   
        std::cerr << "./ircserv <port> <password>" << "\n";
        return 1;
    }
    try {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT,Server::receve_signal);
        signal(SIGQUIT,Server::receve_signal);

        std::string port = av[1];
        std::string password = av[2];
        Server server(port,password);
        server.StartServer();    
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() <<std::endl;
    }    
    close_all_fds();
} 
