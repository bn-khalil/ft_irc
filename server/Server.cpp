#include "Server.hpp"
#include "../exception/SocketFailedToCreatException.hpp"
#include "../exception/FcntlFailedException.hpp"
#include "../exception/SocketBindFailedException.hpp"
#include "../exception/SocketOptionFailedException.hpp"
#include "../exception/SocketListenFailedException.hpp"
#include "Client.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <sstream>
#include <strings.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include "../commands/channel_membership/channel.hpp"

int Server::ReadClientMessage(std::string &line)
{
    (void)line;
    return 0;
}
void Server::NickCmd(Client &client, std::string nick_arg)
{
    if (client.GetIsSetPass() == false)
    {
        // "464 :Password incorrect
        return ;
    }
    else if (nick_arg.empty())
     {
        //"431 :No nickname given"
        return ;
     }
    else 
    {
        for (std::map<int, Client>::iterator it = ClientsInfo.begin(); it != ClientsInfo.end(); ++it)
        {
         if (it->first == client.getfd())
        {   
            continue;
        }
            if (it->second.GetIsSetNick() == true &&it->second.get_nickname()==nick_arg)
            {
                return ;
                // "433  Nickname is already in use");
            }
        }
    }
    std::cout << "NICK name is seted " << nick_arg << std::endl;
    client.set_nickname(nick_arg);
    client.SetIsSetNick(true);
}


int Server::stringToPort(std::string &string)
{
    int port = -1;
    std::stringstream str_strm;
    str_strm << string;
    str_strm >> port;

    if (str_strm.fail() || !str_strm.eof())
        return -1;
    else if (port >= 1024 && port <= 49151)
        return port;
    return -1;
}

Server::Server(void) : password("0123456789") {}

void Server::StartServer()
{
    try
    {
        PrepareServerSocket();
        waitConnection();
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

Server::Server(const Server &other)
{
    (void)other;
}

void Server::AddClient()
{
    int ClientSocketFd = accept(serverId, NULL, NULL);
    if (ClientSocketFd < 0)
    {
        std::cerr << "ClientSocketFd \n";
        return;
    }
    if (fcntl(ClientSocketFd, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "FcntlFailedException  \n";
        close(ClientSocketFd);
        return;
    }

    pollfd ClientPollfd;
    ClientPollfd.fd = ClientSocketFd;
    ClientPollfd.events = POLLIN;
    ClientPollfd.revents = 0;

    poll_fds.push_back(ClientPollfd);

    Client client(ClientSocketFd);
    ClientsInfo[ClientSocketFd] = client;

    std::cout << "client number " << ClientSocketFd << " connect" << std::endl;
}

std::vector<std::string> Server::splitCmd(std::string &str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (ss >> item)
        result.push_back(item);

    return result;
}

void Server::PassCmd(Client &client, std::string password_arg)
{
    std::cout << password_arg.empty() << std::endl;
    if (client.Get_isAuthenticated() == true)
    {
        // client.getFd(),
        return;
        // "462 :You may not reregister");
    }
    if (password_arg.empty())
    {
        // "461 PASS :Not enough parameters"
        return;
    }
    if (password_arg != this->password)
    {
        return;
        // "464 :Password incorrect"
    }
    std::cout << client.getfd() << " " << "daz \n";
    client.SetIsSetPass(true);
}

void Server::ParseCmd(Client &client)
{
    std::vector<std::string> cmds;

    cmds = splitCmd(client.getlineCmd());

    std::string empty = "";
    client.setlineCmd(empty);

    if (cmds.size() == 0)
        return;

    if (cmds[0] == "PASS")
    {
        if (client.Get_isAuthenticated() == true)
        {
        }
        PassCmd(client, cmds[1]);
        std::cout << "PASS commmand" << std::endl;
    }
    else if (cmds[0] == "NICK")
    {
        Server::NickCmd(client,cmds[1]);
        std::cout << "NICK commmand" << std::endl;
    }

    else if (cmds[0] == "JOIN" || cmds[0] == "join")
    {
        join(cmds, client);
    }
    // else if(cmds[0] == "KICK" || cmds[0] == "kick")
    // {

    // }
    // else if(cmds[0] == "INVIT" || cmds[0] == "invit")
    // {

    else if (cmds[0] == "USER")
    {
        std::cout << "USER commmand" << std::endl;
    }
}

void Server::GetClientEvents()
{
    for (size_t i = 0; i < poll_fds.size(); i++)
    {
        if (poll_fds[i].revents & POLLIN)
        {
            if (i == 0)
                AddClient();
            else
            {
                char buffer[1024];
                bzero(buffer, 1024);
                std::string str_buffer;

                Client &client = ClientsInfo[poll_fds[i].fd];
                int bytes_read = recv(poll_fds[i].fd, buffer, sizeof(buffer) - 1, 0);

                if (bytes_read > 0)
                {
                    std::string str_buffer(buffer, bytes_read);

                    if (str_buffer.find('\n') == std::string::npos)
                    {
                        std::cout << "client.getfd()" << client.getfd() << "\n";
                        client.setlineCmd(client.getlineCmd().append(str_buffer));
                    }
                    else
                    {
                        client.setlineCmd(client.getlineCmd().append(str_buffer));
                        ParseCmd(client);
                    }
                }
                else if (bytes_read == 0)
                {
                    std::cout << "client number " << poll_fds[i].fd << " disconnect" << std::endl;
                    close(poll_fds[i].fd);
                    ClientsInfo.erase(poll_fds[i].fd);
                    poll_fds.erase(poll_fds.begin() + i);
                    i--;
                }
                else
                {
                }
            }
        }
    }
}

void Server::waitConnection()
{
    while (true)
    {
        int num_event = poll(&poll_fds[0], poll_fds.size(), 0);

        if (num_event == -1)
        {
            perror("poll error");
            continue;
        }
        else if (num_event == 0)
        {
            // perror("timeout");
            continue;
        }
        else if (num_event > 0)
        {
            GetClientEvents();
        }
    }
}

void Server::PrepareServerSocket()
{
    serverId = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (serverId < 0)
        throw SocketFailedToCreatException();
    if (setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw SocketOptionFailedException();
    if (fcntl(serverId, F_SETFL, O_NONBLOCK) < 0)
        throw FcntlFailedException();

    serverConfig.sin_family = AF_INET;
    serverConfig.sin_port = htons(port);
    serverConfig.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverId, (struct sockaddr *)(&serverConfig), sizeof(serverConfig)) < 0)
        throw SocketBindFailedException();
    if (listen(serverId, SOMAXCONN) < 0)
        throw SocketListenFailedException();

    struct pollfd ServerPollfd;
    ServerPollfd.fd = serverId;
    ServerPollfd.events = POLLIN;
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

Server::~Server(void) {}

Server::Server(std::string &port, std::string &password)
    : port(stringToPort(port)), password(password), isGetSignal(false)
{
    serverId = -1;
    addr_len = sizeof(sockaddr_in);
}






//-------------------------------------------------------------------CHANNEL_PART----------------------------------------------------------------------------------------------
void Server::removeClientFromAllChannels(Client &c)
{
    std::vector<std::string> channel_to_leave;
    std::map<std::string, Channel *>::iterator it = this->channel.begin();
    while(it != this->channel.end())
    {
        Channel *ch = it->second;
        if(ch->isUserInChannel(c))
            channel_to_leave.push_back(ch->get_channel_name());
        it++;
    }
    for(size_t i = 0; i < channel_to_leave.size();i++)
    {
        std::map<std::string,Channel*>::iterator it1 = channel.find(channel_to_leave[i]);
        if(it1 == channel.end())
            continue;
        Channel *chan = it1->second;
         chan->broadcast( error.MSG_PART(c.get_Prefix(), chan->get_channel_name(), "Leaving") );
        chan->rm_user_from_channel(c);
       if(chan->isEmpty() == true)
       {
            channel.erase(it1);
            delete chan;
       }
    }
    
}

void Server::sendReply(Client &c, std::string msg)
{
    
    std::string full_msg = msg + "\r\n";
    (void) c;
    if(send(c.getfd() , full_msg.c_str() , full_msg.length(), 0) <= -1)
    {
        std::cerr << "Client Disconnected" << std::endl;
    }
    
}