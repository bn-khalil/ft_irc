#include "Server.hpp"
#include "../commands/channel_membership/Reply.hpp"
#include "Client.hpp"
#include <cstddef>
#include <ctime>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
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
#include <stdio.h>
#include <errno.h>

bool Server::isGetSignal = false;

bool Server::get_Signal(void)
{
    return (isGetSignal );
}
void Server::set_Signal(bool sign)
{
    isGetSignal = sign;
}

void Server::receve_signal(int sign)
{
    isGetSignal = true;
    if (sign == SIGINT)
        std::cout << "\n\n \033[31m Interrupted by SIGINT \033[0m\n" << std::endl;
    else if (sign == SIGQUIT)
        std::cout << "\n\n \033[31m Interrupted by SIGQUIT \033[0m\n" << std::endl;
    else
        std::cout << "\n\n \033[31m Interrupted by signal \033[0m\n" << std::endl;


}
int Server::ReadClientMessage(std::string &line)
{
    (void)line;
    return 0;
}

bool isValidNickname(const std::string &nick)
{
    if (nick.empty())
        return false;

    std::string forbiddenStartChars = "0123456789$:#&";

    if (forbiddenStartChars.find(nick[0]) != std::string::npos)
        return false;

    std::string forbiddenChars = " ,*?!@";

    if (nick.find_first_of(forbiddenChars) != std::string::npos)
        return false;

    return true;
}

void Server::NickCmd(Client &client, std::string nick_arg)
{
    if (client.Get_isAuthenticated() == true)
    {
        sendReply(client, error.ERR_ALREADYREGISTERED(client.get_nickname(), ":You may not reregister"));
        return;
    }
    else if (client.GetIsSetPass() == false)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(), ":Password not set"));
        return;
    }
    else if (!isValidNickname(nick_arg))
    {
        sendReply(client, error.ERR_ERRONEUSNICKNAME(client.get_nickname(), nick_arg, ":Erroneus nickname"));
    }
    else if (nick_arg.empty())
    {
        sendReply(client, error.ERR_NONICKNAMEGIVEN(client.get_nickname(), ":No nickname given"));
        return;
    }
    else
    {
        for (std::map<int, Client>::iterator it = ClientsInfo.begin(); it != ClientsInfo.end(); ++it)
        {
            if (it->first == client.getfd())
                continue;
            if (it->second.GetIsSetNick() == true && toLower (it->second.get_nickname()) == toLower(nick_arg))
            {
                sendReply(client, error.ERR_NICKNAMEINUSE(client.get_nickname(), nick_arg, ":Nickname is already in use"));
                return;
            }
        }
    }
    client.set_nickname(nick_arg);
    client.SetIsSetNick(true);
    if (client.GetIsSetuser() == true)
    {
        sendReply(client, error.RPL_WELCOME(client.get_nickname(), client.get_Prefix()));
        sendReply(client, error.RPL_YOURHOST(client.get_nickname()));
        sendReply(client, error.RPL_CREATED(client.get_nickname(), creation_date));
        sendReply(client, error.RPL_MYINFO(client.get_nickname()));
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

void Server::StartServer()
{
    try
    {
        PrepareServerSocket();
        std::cout << "\n\033[35mServer is now running and ready!\033[0m" << std::endl;
        std::cout << "\033[96m\n██╗██████╗░░█████╗░░██████╗███████╗██████╗░██╗░░░██╗███████╗██████╗░\n██║██╔══██╗██╔══██╗██╔════╝██╔════╝██╔══██╗██║░░░██║██╔════╝██╔══██╗\n██║██████╔╝██║░░╚═╝╚█████╗░█████╗░░██████╔╝╚██╗░██╔╝█████╗░░██████╔╝\n██║██╔══██╗██║░░██╗░╚═══██╗██╔══╝░░██╔══██╗░╚████╔╝░██╔══╝░░██╔══██╗\n██║██║░░██║╚█████╔╝██████╔╝███████╗██║░░██║░░╚██╔╝░░███████╗██║░░██║\n╚═╝╚═╝░░╚═╝░╚════╝░╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝\033[0m\n";
        std::cout <<  "\033[32mPort        : " << port     << "\033[0m" << std::endl;
        std::cout << "\033[33m" << "Password    : " << password << "\033[0m\n" << std::endl;
        waitConnection();
    }
    catch (std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

void Server::AddClient()
{
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
    client.set_hostname(hostname);
    //need to add username
    std::cout << "🟢 New Client Connected | FD: " << ClientSocketFd << std::endl;
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
    if (client.Get_isAuthenticated() == true)
    {
        sendReply(client, error.ERR_ALREADYREGISTERED(client.get_nickname(), ":You may not reregister"));
        return;
    }
    if (password_arg.empty())
    {
        sendReply(client, error.ERR_NEEDMOREPARAMS(client.get_nickname(), "PASS"));
        client.SetIsSetPass(false);
        return;
    }
    if (password_arg != this->password)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(), ":Password incorrect"));
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
        sendReply(client, error.ERR_ALREADYREGISTERED(client.get_nickname(), ":You may not reregister"));
        return;
    }
    else if (size_cmd < 5)
    {
        sendReply(client, error.ERR_NEEDMOREPARAMS(client.get_nickname(), "USER"));
        return;
    }
    else if (client.GetIsSetPass() == false)
    {
        sendReply(client, error.ERR_PASSWDMISMATCH(client.get_nickname(), ":Password not set"));
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
        sendReply(client, error.RPL_WELCOME(client.get_nickname(), client.get_Prefix()));
        sendReply(client, error.RPL_YOURHOST(client.get_nickname()));
        sendReply(client, error.RPL_CREATED(client.get_nickname(), creation_date));
        sendReply(client, error.RPL_MYINFO(client.get_nickname()));
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

    if (client.getlineCmd().size() >= 512)
    {
        sendReply(client, error.ERR_INPUTTOOLONG(client.get_nickname()));
        return;
    }

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
    else if (cmds[0] == "quit")
        Quit(client);
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
        UserCmd(client, cmds);
    else if (cmds[0] == "pong" || cmds[0] == "ping")
    {
        if (cmds[0] == "ping")
        {
            std::string token = (cmds.size() > 0) ? cmds[0] : "";
            std::string reply = "PONG " + token + "\r\n";
            send(client.getfd(), reply.c_str(), reply.length(), 0);
        }
    }
    else
        sendReply(client, error.ERR_UNKNOWNCOMMAND_N(client.get_nickname(), cmds[0]));

    std::string empty = "";
    client.setlineCmd(empty);
    cmds.clear();
}

void Server::processClientBuffer(Client &client, char *buffer, int bytes_read)
{
    std::string str_buffer(buffer, bytes_read);

    if (str_buffer.find('\n') == std::string::npos)
        client.setlineCmd(client.getlineCmd().append(str_buffer));
    else
    {
        client.setlineCmd(client.getlineCmd().append(str_buffer));
        std::string s = client.getlineCmd();

        for (std::string::size_type pos = 0; (pos = s.find("\r\n", pos)) != std::string::npos;)
            s.replace(pos, 2, "\n");

        client.setlineCmd(s);
        std::vector<std::string> cmds = new_splite(client.getlineCmd(), '\n');
        int client_fd = client.getfd();

        for (size_t i = 0; i < cmds.size(); i++)
        {
            if (ClientsInfo.find(client_fd) == ClientsInfo.end())
                break ;
            client.setlineCmd(cmds[i]);
            ParseCmd(client);
        }
    }
}

void Server::GetClientEvents()
{
    for (size_t i = 0; i < poll_fds.size(); i++)
    {
        if (poll_fds[i].revents & POLLIN)
        {
            if (poll_fds[i].fd == this->serverId)
                AddClient();
            else
            {
                char buffer[1024];
                bzero(buffer, 1024);

                Client &client = ClientsInfo[poll_fds[i].fd];
                int bytes_read = recv(poll_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                if (bytes_read > 0)
                    processClientBuffer(client, buffer, bytes_read);
                else if (bytes_read == 0)
                {
                    std::cout << "🔴 Client Disconnected | FD: " << client.getfd() << " | Nick: " << client.get_nickname() << std::endl;
                    Quit(client);
                    i--;
                }
                else if (bytes_read < 0 && (errno != EAGAIN && errno != EWOULDBLOCK))
                {
                    std::cerr << "🔴 Recv Failed | FD: " << client.getfd() << " | Error: " << strerror(errno) << std::endl;
                    Quit(client); 
                    i--; 
                }
            }
        }
    }
}

void Server::waitConnection()
{
    while (isGetSignal != true)
    {        
        int num_event = poll(&poll_fds[0], poll_fds.size(), -1);
        if (num_event == -1 && errno != EINTR)
            throw std::runtime_error("poll failed");
        else if (num_event == 0)
            continue;
        else if (num_event > 0)
            GetClientEvents();
   }
}

void Server::PrepareServerSocket()
{
    serverId = socket(AF_INET, SOCK_STREAM, 0);
    if (serverId < 0)
        throw std::runtime_error("Error: Failed to create socket: " + std::string(strerror(errno)));

    int opt = 1;

    if (setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        close(serverId);
        throw std::runtime_error("Error: setsockopt(SO_REUSEADDR) failed: " + std::string(strerror(errno)));
    }

    if (fcntl(serverId, F_SETFL, O_NONBLOCK) < 0)
    {
        close(serverId);
        throw std::runtime_error("Error: fcntl(O_NONBLOCK) failed: " + std::string(strerror(errno)));
    }

    serverConfig.sin_family = AF_INET;
    serverConfig.sin_port = htons(port);
    serverConfig.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverId, (struct sockaddr *)(&serverConfig), sizeof(serverConfig)) < 0)
    {
        close(serverId);
        throw std::runtime_error("Error: Bind failed: " + std::string(strerror(errno)));
    }

    if (listen(serverId, SOMAXCONN) < 0)
    {
        throw std::runtime_error("Error: Listen failed: " + std::string(strerror(errno)));
        close(serverId);
    }

    struct pollfd ServerPollfd;
    ServerPollfd.fd = serverId;
    ServerPollfd.events = POLLIN;
    ServerPollfd.revents = 0;

    poll_fds.push_back(ServerPollfd);
}

Server::~Server(void) {}

bool isValidPassword(std::string str)
{
    std::string W_spaces = " \t\n\r\v\f";
    if (str.empty() || str.length() > 100)
        return false;

    for(int i = 0; str[i] ; i++)
    {
        if (W_spaces.find(str[i]) != std::string::npos)
            return false;
    }
    return true;
}


Server::Server(std::string &port, std::string &password)
    : port(stringToPort(port)), password(password)
{
    if (this->port < 0)
        throw std::runtime_error("Error: Invalid port number provided");
    else if (isValidPassword(password) == false)
        throw std::runtime_error("Error: Invalid password format ");
    
    std::time_t time = std::time(NULL);
    std::tm* localTime = std::localtime(&time);
    char buffer[30];
    bzero(buffer, sizeof(buffer));
    std::strftime(buffer,sizeof(buffer),"%Y-%m-%d | %H:%M:%S",localTime);
    creation_date = buffer;
    serverId = -1;
}

void Server::removeClientFromAllChannels(Client &c)
{
    std::vector<std::string> channel_to_leave;
    std::map<std::string, Channel>::iterator it = this->channel.begin();

    while (it != this->channel.end())
    {
        Channel &ch = it->second;
        if (ch.isUserInChannel(c) == true)
            channel_to_leave.push_back(ch.get_channel_name());
        it++;
    }

    for (size_t i = 0; i < channel_to_leave.size(); i++)
    {
        std::map<std::string, Channel>::iterator it1 = channel.find(channel_to_leave[i]);
        if (it1 == channel.end())
            continue;
        Channel &chan = it1->second;
        chan.broadcast(error.MSG_PART(c.get_Prefix(), chan.get_channel_name(), "Left all channels"));
        chan.removeClientFromOneChannels(c);
        if (chan.isEmpty() == true)
            channel.erase(it1);
    }
}

void Server::sendReply(Client &c, std::string msg)
{
    std::string full_msg = msg + "\r\n";
    if (send(c.getfd(), full_msg.c_str(), full_msg.length(), 0) <= -1)
    {
        std::cout << "🔴 Client Disconnected | FD: " << c.getfd() << " | Nick: " << c.get_nickname() << std::endl;        
        removeClientFromAllChannels(c);
        close(c.getfd());
        ClientsInfo.erase(c.getfd());
    }
}

Client *Server::find_client_by_nickname(std::string nick)
{
    for (std::map<int, Client>::iterator it = ClientsInfo.begin(); it != ClientsInfo.end(); it++)
    {
        if ( toLower(it->second.get_nickname()) == toLower(nick) )
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
std::string  Server::handle_the_resone(std::vector<std::string>cmds, std::string command, Client &c)
{
     std::string reason = c.get_nickname();
    if (cmds.size() > 3)
    {
        if (cmds[3][0] == ':')
        {
            if (command.find(cmds[3], command.find(cmds[2], command.find(cmds[1]) + cmds[1].length()) + cmds[2].length()) != std::string::npos)
                reason = command.substr(command.find(cmds[3], command.find(cmds[2], command.find(cmds[1]) + cmds[1].length()) + cmds[2].length()) + 1);
        }
        else
            reason = cmds[cmds.size() - 1];
    }
    return reason;
}
void  Server::kick_with_brodcast(std::map<std::string, Channel>::iterator it, std::vector<std::string>cmds, std::string reason, Client & c)
{
        it->second.broadcast(error.MSG_KICK(c.get_Prefix(), it->second.get_channel_name(), cmds[2], reason));
        it->second.removeClientFromOneChannels(*find_client_by_nickname(cmds[2]));
        if(it->second.isEmpty())
            channel.erase(it); 
}
bool   Server::name_perfect(std::vector<std::string> cmds, std::map<std::string, Channel>::iterator it)
{
    return cmds[1].empty() || (cmds[1][0] != '&' && cmds[1][0] != '#') || cmds[1].length() > 200 || Channel::tab_found(cmds[1]) == true  || it == channel.end();
}