#include "server/Server.hpp"
// #include "server/Authentication.hpp"
// #include "server/Client.hpp"
#include "exception/SocketFailedToCreatException.hpp"
#include "exception/FcntlFailedException.hpp"
#include "exception/SocketBindFailedException.hpp"
#include "exception/SocketOptionFailedException.hpp"
#include "exception/SocketListenFailedException.hpp"
#include <cstddef>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <vector>

int main(int ac,char **av)
{
    
    (void)av;
    if (ac != 3 )
    {   
        std::cerr << "./ircserv <port> <password>" << "\n";
        return 1;
    }
    std::string port = av[1];
    std::string password = av[2];
    Server server(port,password);
    server.StartServer();
    // std::cout << "the content of buffer is :" <<  server.get_buffer()  << std::endl;


    
} 
