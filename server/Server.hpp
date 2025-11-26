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

typedef struct modes_s {
    std::string param;
    char mode;
    bool sing;
} modes_t;

class Client;
class Channel;
class Server
{
    private:

        int						port;
        int						serverId;
        std::string             password;
        static bool                    isGetSignal;
        std::string creation_date;
        socklen_t addr_len;
        sockaddr_in  serverConfig;
        std::vector<struct pollfd> poll_fds;
        std::map<int, Client>	ClientsInfo;
        int ReadClientMessage(std::string &line);
        void AddClient();
        void GetClientEvents();
        void processClientBuffer(Client &client, char *buffer, int bytes_read);
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
        void UserCmd(Client &client, std::vector<std::string> &arg);
        std::vector<std::string> splitCmd(std::string &str);
        void Quit(Client &c);


    public:
        static void set_Signal(bool sign);
        static bool get_Signal(void);
        static void receve_signal(int sign);
		std::map<std::string,Channel> channel;
        Reply   error;
        Channel *chan;
        Server(std::string &port,std::string &password);
        Server(const Server& other);
        
        void  StartServer();
        void  join(Client &c);
        void  kick(Client &c);
        void  invit(Client &c);
        void  mode(Client &c);
        void  topic(Client &c);
        void  privmsg(Client &c);
        void sendReply(Client &c, std::string msg);
        void Clean_resource(Client &c);
        void removeClientFromAllChannels(Client &c);
        std::string toLower(std::string str);
        void join_all_channel(Client &c);
        Client *find_client_by_nickname(std::string nick);
       std::vector<std::string>  new_splite(std::string &strr, char d);
       bool isChannelExist(std::map<std::string,Channel>::iterator & it_channel, 
        std::vector<std::string> cmds, Client &c);
        std::vector<modes_t> parseModes(std::vector<std::string> cmds,  
        Client &c) ;
       std::vector<std::string> isUserInvited_to_channel(Client &c);

        ~Server();
};

#endif
	 