#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>

class Client
{
        int fd;
        bool isAuthenticated;
        std::string _nickname;
        std::string _username;
        std::string lineCmd;
    public:
        void setlineCmd( std::string &line)
        {
            lineCmd = line;
        }
        std::string &getlineCmd()
        {
            return lineCmd;
        }
        Client(void);
        Client(const Client& other);
        Client(int fd);
        Client &operator=(const Client &other);
        ~Client();
};


#endif

