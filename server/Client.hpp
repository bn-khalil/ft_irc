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
        bool isSetPass;
        bool isSetNick;
        // bool isSetPass;
        // bool isSetPass;

    public:
        void setlineCmd( std::string &line)
        {
            lineCmd = line;
        }
        std::string &getlineCmd()
        {
            return lineCmd;
        }
        void setfd( int line)
        {
            fd = line;
        }
        int getfd()
        {
            return fd;
        }
        void Set_isAuthenticated( bool flag)
        {
            isAuthenticated = flag;
        }
        int Get_isAuthenticated()
        {
            return fd;
        }
        void SetIsSetPass(bool flag)
        {
            isSetPass = flag ;
        }
        bool GetIsSetPass(){
            return isSetPass;
        }
        void SetIsSetNick(bool flag)
        {
            isSetNick= flag ;
        }
        bool GetIsSetNick(){
            return isSetNick;
        }
        
        Client(void);
        Client(const Client& other);
        Client(int fd);
        Client &operator=(const Client &other);
        ~Client();
};


#endif

