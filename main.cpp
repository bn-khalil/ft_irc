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
int ServerFd;

int main(int ac,char **av)
{
    
    (void)av;
    if (ac != 3 )
    {   
        std::cerr << "./ircserv <port> <password>" << "\n";
        return 1;    
    }
    //creat socket
    Server server();


    // ServerFd = socket(AF_INET, SOCK_STREAM, 0);
    // if (ServerFd < 0)
    // {   
    //     // throw SocketFailedToCreatException();
    // }
    // else 
    //     std::cout << "Socket created successfully. fd = " << ServerFd << std::endl;
    // // config socket // and non blocking socket
    // int opt = 1;
    // if (setsockopt(ServerFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))< 0)
    // {
    //     // throw SocketOptionFailedException();
    // }
    // if (fcntl(ServerFd,F_SETFL,O_NONBLOCK)< 0)
    // {
    //     // throw FcntlFailedException();
    // }
    // // bind and listen
    // sockaddr_in to_bind, client_addr;
    // socklen_t addr_len = sizeof(client_addr); 
    // to_bind.sin_family = AF_INET;
    // to_bind.sin_port =htons(6667); // port in arg
    // to_bind.sin_addr.s_addr = INADDR_ANY;
    // if (bind(ServerFd, (struct sockaddr *)(&to_bind), sizeof(to_bind)) < 0)
    // {
    //     // throw SocketBindFailedException();
    // }
    // if (listen(ServerFd, SOMAXCONN) < 0)
    // {
    //     // throw SocketListenFailedException();
    // }
    // std::vector<struct pollfd> poll_fds;
    // struct pollfd s_pollfd;

    // s_pollfd.fd= ServerFd;
    // s_pollfd.events=POLLIN;
    // s_pollfd.revents = 0;
    // poll_fds.push_back(s_pollfd);
    // while (true)
    // {
    //     int num_event = poll(&poll_fds[0],poll_fds.size(),10);
    //     if (num_event == -1) 
    //     {
    //         perror("poll error");
    //         continue;
    //     }
    //     else if (num_event == 0)
    //     {
    //         perror("timeout");
    //         continue;
    //     }
    //     else if (num_event >0)
    //     {
    //         for (size_t i = 0; i<poll_fds.size();i++)
    //         {
    //             if (poll_fds[i].revents & POLLIN)
    //             {
    //                 int client_socket_fd =accept(ServerFd, (struct sockaddr *)&client_addr, &addr_len);
    //                 if (client_socket_fd == -1 || fcntl(client_socket_fd, F_SETFL, O_NONBLOCK) == -1)
    //                 {
    //                     continue;
    //                 }
    //                 struct pollfd new_client_pollfd;
    //                 new_client_pollfd.fd = client_socket_fd;
    //                 new_client_pollfd.events = POLLIN;
    //                 new_client_pollfd.revents = 0;
    //                 poll_fds.push_back(new_client_pollfd);
    //             }
    //         }
    //     }

    // }

} 
