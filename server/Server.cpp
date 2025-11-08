#include "Server.hpp"
#include "../exception/SocketFailedToCreatException.hpp"
#include "../exception/FcntlFailedException.hpp"
#include "../exception/SocketBindFailedException.hpp"
#include "../exception/SocketOptionFailedException.hpp"
#include "../exception/SocketListenFailedException.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <vector>

int Server::stringToPort(std::string &string)
{
    int port = -1;
    std::stringstream str_strm;
    str_strm << string;
    str_strm >> port;
    if (str_strm.fail() || !str_strm.eof())
        return -1;
    else if (port >= 1024 &&  port <= 49151)
        return port;
    return -1;
}

Server::Server(void):password("0123456789")
{

}
void  Server::StartServer()
{
    try {
        PrepareServerSocket();
        waitConnection();
    }
    catch(std::exception &ex)
    {
        std::cerr <<  ex.what() << std::endl;
    }
}

Server::Server(const Server &other)
{
    (void) other;
}
void Server::waitConnection()
{
    while (true)
    {
        int num_event = poll(&poll_fds[0],poll_fds.size(),-1);
        //exception later hh
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
                    if (i == 0) // server event
                    {
                        int ClientSocketFd =accept(serverId, (struct sockaddr *)&ClientsInfo, &addr_len);
                        if (ClientSocketFd < 0)
                                std::cerr << "ClientSocketFd < 0 \n";                    
                        if (fcntl(ClientSocketFd, F_SETFL, O_NONBLOCK) < 0)
                                throw FcntlFailedException();

                        pollfd ClientPollfd;
                        ClientPollfd.fd = ClientSocketFd;
                        ClientPollfd.events = POLLIN;
                        ClientPollfd.revents=0;
                        poll_fds.push_back(ClientPollfd);
                        std::cout  << "add client  \n"; 
                    }
                    else
                    {
                        char buffer[1024];
                        int bytes_read = recv(poll_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                        if (bytes_read > 0)
                        {
                            buffer[bytes_read] = '\0';
                            std::cout << buffer << std::endl;
                            // std::cout << "buffer\n";
                        }
                        else if (bytes_read == 0)
                            ; // later
                        else
                         ; //error
                    }
        }
    }
}
}}
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
    struct pollfd ServerPollfd;
    ServerPollfd.fd= serverId;
    ServerPollfd.events=POLLIN;
    ServerPollfd.revents = 0;
    poll_fds.push_back(ServerPollfd);
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

Server::Server(std::string &port,std::string &password):port(stringToPort(port)),password(password),isGetSignal(false)
{
    serverId = -1;
    addr_len = sizeof(sockaddr_in);
}