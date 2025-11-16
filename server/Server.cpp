#include "Server.hpp"
#include "../commands/channel_membership/Reply.hpp"
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
#include <string>
#include <strings.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include "../commands/channel_membership/channel.hpp"
#include <arpa/inet.h>

int Server::ReadClientMessage(std::string &line)
{
    (void)line;
    return 0;
}

void Server::NickCmd(Client &client, std::string nick_arg)
{
    // parse the nick name  --> ERR_ERRONEUSNICKNAME 432   "<client> <nick> :Erroneus nickname"
    if (client.GetIsSetPass() == false)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(),":Password not set"));
        return;
    }
    else if (nick_arg.empty())
    {
        sendReply(client, error.ERR_NONICKNAMEGIVEN(client.get_nickname(),":No nickname given"));
        return;
    }
    else
    {
        for (std::map<int, Client>::iterator it = ClientsInfo.begin(); it != ClientsInfo.end(); ++it)
        {
            if (it->first == client.getfd())
                continue;
            if (it->second.GetIsSetNick() == true && it->second.get_nickname() == nick_arg)
            {
                sendReply(client, error.ERR_NICKNAMEINUSE(client.get_nickname(), nick_arg,":Nickname is already in use"));
                return;
            }
        }
    }
    client.set_nickname(nick_arg);
    client.SetIsSetNick(true);
    if (client.GetIsSetuser() == true)
    {
        std::cout << " authentication dazet nick set\n";
        client.Set_isAuthenticated(true);
    }
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
    // ------------------fixprefix---------------------
    sockaddr_in client_addr;
    socklen_t addr_len;

    int ClientSocketFd = accept(serverId, (struct sockaddr *)&client_addr, &addr_len);
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
    std::string hostname = inet_ntoa(client_addr.sin_addr);

    pollfd ClientPollfd;
    ClientPollfd.fd = ClientSocketFd;
    ClientPollfd.events = POLLIN;
    ClientPollfd.revents = 0;

    poll_fds.push_back(ClientPollfd);

    Client client(ClientSocketFd);
    client.set_hostname(hostname);
    ClientsInfo[ClientSocketFd] = client;
    std::cout << client.get_hostname() << std::endl;

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
    std::cout << "password_arg " << password_arg << std::endl;
    if (client.Get_isAuthenticated() == true)
    {
        sendReply(client, error.ERR_ALREADYREGISTERED(client.get_nickname(),":You may not reregister"));
        client.SetIsSetPass(false);
        return;
    }
    if (password_arg.empty())
    {
        sendReply(client, error.ERR_NEEDMOREPARAMS(client.get_nickname(),"PASS"));
        client.SetIsSetPass(false);
        return;
    }
    if (password_arg != this->password)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(),":Password incorrect"));
        client.SetIsSetPass(false);
        return;
    }
    client.SetIsSetPass(true);
}

void Server::UserCmd(Client &client, std::vector<std::string> &arg)
{
    std::string userName;
    std::string realName;

    size_t size_cmd = arg.size();
    size_t two = client.getlineCmd().find(" :");
    if (client.Get_isAuthenticated() == true)
    {
        sendReply(client, error.ERR_ALREADYREGISTERED(client.get_nickname(),":You may not reregister"));
        return;
    }
    else if (size_cmd < 5)
    {
        sendReply(client, error.ERR_NEEDMOREPARAMS(client.get_nickname(),"USER"));
        return;
    }
    else if (client.GetIsSetPass() == false)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(),":Password not set"));
        return;
    }
    userName = arg[1];
    if (two == std::string::npos)
        realName = arg[4];
    else
        realName = client.getlineCmd().substr(two + 2);

    client.SetIsSetuser(true);
    client.set_realname(realName);
    client.set_username(userName);
    if (client.GetIsSetNick() == true)
    {
        std::cout << " authentication dazet  \n";
        client.Set_isAuthenticated(true);
    }
}

std::string Server::toLower(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
        str[i] = std::tolower(str[i]);
    return str;
}

void Server::ParseCmd(Client &client)
{
    std::vector<std::string> cmds;
    cmds = new_splite(client.getlineCmd(), ' ');

    if (cmds.size() == 0)
        return;
    cmds[0] = toLower(cmds[0]);

    if (cmds[0] == "pass")
        PassCmd(client, cmds[1]);
    else if (cmds[0] == "nick")
        Server::NickCmd(client, cmds[1]);
    else if (cmds[0] == "join")
        join(client);
    else if (cmds[0] == "topic")
        topic(client);
    else if (cmds[0] == "privmsg")
        privmsg(client);
    else if (cmds[0] == "mode")
        mode(client);
    else if (cmds[0] == "kick")
        kick(client);
    else if (cmds[0] == "invite")
        invit(client);
    else if (cmds[0] == "user")
    {
        UserCmd(client, cmds);
        std::cout << "USER commmand" << std::endl;
    }
    else
        sendReply(client, error.ERR_UNKNOWNCOMMAND_N(client.get_nickname(), cmds[0]));

    std::string empty = "";
    client.setlineCmd(empty);
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
                        std::string s = client.getlineCmd();

                        for (std::string::size_type pos = 0; (pos = s.find("\r\n", pos)) != std::string::npos;)
                            s.replace(pos, 2, "\n");

                        client.setlineCmd(s);
                        std::vector<std::string> cmds = new_splite(client.getlineCmd(), '\n');
                        for (size_t i = 0; i < cmds.size(); i++)
                        {
                            client.setlineCmd(cmds[i]);
                            ParseCmd(client);
                        }
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

// -------------------------------------------------------------------CHANNEL_PART----------------------------------------------------------------------------------------------

void Server::removeClientFromAllChannels(Client &c)
{
    std::vector<std::string> channel_to_leave;
    std::map<std::string, Channel>::iterator it = this->channel.begin();
    while (it != this->channel.end())
    {
        Channel ch = it->second;
        if (ch.isUserInChannel(c))
            channel_to_leave.push_back(ch.get_channel_name());
        it++;
    }
    for (size_t i = 0; i < channel_to_leave.size(); i++)
    {
        std::map<std::string, Channel>::iterator it1 = channel.find(channel_to_leave[i]);
        if (it1 == channel.end())
            continue;
        Channel chan = it1->second;
        chan.broadcast(error.MSG_PART(c.get_Prefix(), chan.get_channel_name(), "Left all channels"));
        chan.rm_user_from_channel(c);
        if (chan.isEmpty() == true)
            channel.erase(it1);
    }
}

void Server::sendReply(Client &c, std::string msg)
{
    std::string full_msg = msg + "\r\n";
    if (send(c.getfd(), full_msg.c_str(), full_msg.length(), 0) <= -1)
    {
        std::cerr << "Client Disconnected" << std::endl;
    }
}

Client *Server::find_client_by_nickname(std::string nick)
{
    for (std::map<int, Client>::iterator it = ClientsInfo.begin(); it != ClientsInfo.end(); it++)
    {
        if (it->second.get_nickname() == nick)
            return &it->second;
    }
    return NULL;
}

std::vector<std::string> Server::new_splite(std::string &strr, char d)
{
    std::string save;
    std::stringstream ss(strr);
    std::vector<std::string> resulte;

    while (getline(ss, save, d))
    {
        if (!save.empty())
            resulte.push_back(save);
    }
    return resulte;
}

std::vector<std::string> Server::isUserInvited_to_channel(Client &c)
{
    std::map<std::string, Channel>::iterator it = channel.begin();
    std::vector<std::string> all_channel;
    while (it != channel.end())
    {
        Channel ch = it->second;
        if (ch.isInvited(c))
            all_channel.push_back(it->first);
        it++;
    }
    return all_channel;
}

void Server::join_all_channel(Client &c)
{
    for (std::map<std::string, Channel>::iterator it = channel.begin(); it != channel.end(); it++)
    {
        Channel ch = it->second;
        ch.Add_to_user(c);
    }
}
