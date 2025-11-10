#ifndef SERVER_HPP
# define SERVER_HPP
#include "Client.hpp"
# include <iostream>
#include <poll.h>
#include <map>
#include <string>
#include <vector>
#include <netinet/in.h> 
#include "../commands/channel_membership/channel.hpp"

class Client;

class Server
{
    private:
    // std::string             lineCmd;
    int						port;
    int						serverId;
    std::string             password;
    bool                    isGetSignal;
    std::string _serverName;

    socklen_t addr_len;
    sockaddr_in  serverConfig;//to bind the server fd and client //this contain
    
    std::vector<struct pollfd> poll_fds;
    std::map<int, Client>	ClientsInfo;
    int ReadClientMessage(std::string &line);
    void AddClient();
    void GetClientEvents();
    void PrepareServerSocket(); // -->1 creat  socket  --2 socket option 3 non  blocking socket --- 4
    void waitConnection();
    void ConfigureSocket();
    void bindSocket();
    std::string get_server_name();
    void setupServer();
    int stringToPort(std::string &string);
    void ListenSocket();
    // --- General Errors ---
    std::string ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& command);
    std::string ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name);
    std::string ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel);
    std::string ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel);

    // --- JOIN Part ---
    std::string ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel);
    std::string ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel);
    std::string ERR_CHANNELISFULL(const std::string& nick, const std::string& channel);
    std::string MSG_JOIN(const std::string& user_prefix, const std::string& channel_name);
    std::string RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic);
    std::string RPL_NOTOPIC(const std::string& nick, const std::string& channel);
    std::string RPL_NAMREPLY(const std::string& nick, const std::string& channel, const std::string& names_list);
    std::string RPL_ENDOFNAMES(const std::string& nick, const std::string& channel);
    public:
        void  StartServer();
        Server(void);
        Server(std::string &port,std::string &password);
        Server(const Server& other);
        Server &operator=(const Server &other);
        void  join(std::vector<std::string> cmds, Client *c);
		std::map<std::string,Channel*> channel;
        // void quite_from_all_channel(Client *c , Channel *ch);
        void sendReply(Client *c, std::string msg);
        void removeClientFromAllChannels(Client* c);
        ~Server();
};

#endif
