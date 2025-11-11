#ifndef SERVER_HPP
# define SERVER_HPP
#include "Client.hpp"
// # include <iostream>
#include <poll.h>
#include <map>
#include <string>
#include <vector>
#include <netinet/in.h> 
#include "../commands/channel_membership/Reply.hpp"

class Client;
class Channel;
class Server
{
    private:

    int						port;
    int						serverId;
    std::string             password;
    bool                    isGetSignal;
    socklen_t addr_len;
    sockaddr_in  serverConfig;
    std::vector<struct pollfd> poll_fds;
    std::map<int, Client>	ClientsInfo;

    int ReadClientMessage(std::string &line);
    void AddClient();
    void GetClientEvents();
    void PrepareServerSocket(); // -->1 creat  socket  --2 socket option 3 non  blocking socket --- 4
    void waitConnection();
    void ConfigureSocket();
    void bindSocket();
    void setupServer();
    int stringToPort(std::string &string);
    void ListenSocket();
    void ParseCmd(Client &client);
	void PassCmd(Client &client, std::string password_arg);
	void NickCmd(Client &client, std::string password_ar);
	void UserCmd();
    std::vector<std::string> splitCmd(std::string &str);
    Reply   error;

    public:
    
        void  StartServer();
        Channel *chan;
        Server(void);
        Server(std::string &port,std::string &password);
        Server(const Server& other);
        Server &operator=(const Server &other);
        void  join(std::vector<std::string> cmds, Client &c);
        void  kick(std::vector<std::string> cmds, Client &c);
        void  invit(std::vector<std::string> cmds, Client &c);
        void  topic(std::vector<std::string> cmds, Client &c);
		std::map<std::string,Channel*> channel;
        void sendReply(Client &c, std::string msg);
        void removeClientFromAllChannels(Client &c);    
        // void sendReply(Client &c, std::string msg);
        void broadcast(const std::string &msg);
        ~Server();
};

#endif
	