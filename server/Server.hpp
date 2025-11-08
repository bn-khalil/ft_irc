#ifndef SERVER_HPP
# define SERVER_HPP
#include "Client.hpp"
# include <iostream>
#include <poll.h>
#include <map>
#include <string>
#include <vector>
#include <netinet/in.h> 

class Client;

class Server
{
    private:

    int						port;
    int						serverId;
    std::string             password;
    bool                    isGetSignal;
    socklen_t addr_len;

    sockaddr_in  serverConfig;//to bind the server fd and client //this contain
    
    std::vector<struct pollfd> poll_fds;
    std::map<int, Client>	ClientsInfo;
    
    void PrepareServerSocket(); // -->1 creat  socket  --2 socket option 3 non  blocking socket --- 4
    void waitConnection();
    void ConfigureSocket();
    void bindSocket();
    void setupServer();
    int stringToPort(std::string &string);
    void ListenSocket();
    public:
        void  StartServer();
        Server(void);
        Server(std::string &port,std::string &password);
        Server(const Server& other);
        Server &operator=(const Server &other);
        ~Server();
};

#endif
