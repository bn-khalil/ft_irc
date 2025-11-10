#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
#include <string>

class Client
{
        int fd;
        bool isAuthenticated;
        std::string _nickname;
        std::string _username;
        std::string lineCmd;
        // const std::string nickname ;
    public:
        void setlineCmd( std::string &line)
        {
            lineCmd = line;
        }
        std::string &getlineCmd()
        {
            return lineCmd;
        }
        std::string get_nickname() {return _nickname;};
        std::string get_username() {return _username;};
        Client(void);
        Client(const Client& other);
        Client(int fd);
        Client &operator=(const Client &other);
        ~Client();
        std::string  get_Prefix() const;
        int get_fd() const { return fd;};
};


#endif

