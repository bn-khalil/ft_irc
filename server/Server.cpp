#include "Server.hpp"
#include "../exception/SocketFailedToCreatException.hpp"
#include "../exception/FcntlFailedException.hpp"
#include "../exception/SocketBindFailedException.hpp"
#include "../exception/SocketOptionFailedException.hpp"
#include "../exception/SocketListenFailedException.hpp"
#include <cstddef>
#include <iostream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <vector>
Server::Server(void):password("0123456789")
{

}

Server::Server(const Server &other)
{
    (void) other;
}
void Server::waitConnection()
{
    while (true)
    {
        int num_event = poll(&poll_fds[0],poll_fds.size(),10);
        //exption later hh
        if (num_event == -1) 
        {
            perror("poll error");
            continue;
        }
        else if (num_event == 0)
        {
            perror("timeout");
            continue;
        }
        else if (num_event >0)
        {
            for (size_t i = 0; i<poll_fds.size();i++)
            {
                if (poll_fds[i].revents & POLLIN)
                {
                    int client_socket_fd =accept(serverId, (struct sockaddr *)&ClientsInfo, &addr_len);
                    if (client_socket_fd == -1 || fcntl(client_socket_fd, F_SETFL, O_NONBLOCK) == -1)
                    {
                        throw FcntlFailedException();
                    }
                    struct pollfd new_client_pollfd;
                    new_client_pollfd.fd = client_socket_fd;
                    new_client_pollfd.events = POLLIN;
                    new_client_pollfd.revents = 0;
                    poll_fds.push_back(new_client_pollfd);
                }
            }
        }
    }
}
void Server::PrepareServerSocket()
{
    serverId = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if (serverId < 0)
        throw SocketFailedToCreatException();
    if (setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))< 0)
        throw SocketOptionFailedException();
    if (fcntl(serverId,F_SETFL,O_NONBLOCK)< 0)
        throw FcntlFailedException();
    serverConfig.sin_family = AF_INET;
    serverConfig.sin_port =htons(port);
    serverConfig.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverId, (struct sockaddr *)(&serverConfig), sizeof(serverConfig)) < 0)
        throw SocketBindFailedException();
    if (listen(serverId, SOMAXCONN) < 0)
        throw SocketListenFailedException();
    struct pollfd s_pollfd;
    s_pollfd.fd= serverId;
    s_pollfd.events=POLLIN;
    s_pollfd.revents = 0;
    poll_fds.push_back(s_pollfd);
}
Server &Server::operator=(const Server &other)
{
    if (&other != this)
    {

    }
    return (*this);
}

 
Server::~Server(void)
{
}

Server::Server(std::string &port,std::string &password):password(password),port(stringToPort(port)),isGetSignal(false)
{
    serverId = -1;
    addr_len = sizeof(sockaddr_in);
}