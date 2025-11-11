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

    public:
        void setlineCmd( std::string &line)
        {
            lineCmd = line;
        }
        std::string &getlineCmd()
        {
            return lineCmd;
        }
        void setfd( int fd)
        {
            std::cout << fd << " setfd clien(fd)" << std::endl;
            this->fd = fd;
        }
        int getfd()
        {
            return this->fd;
        }
        void Set_isAuthenticated( bool flag)
        {
            isAuthenticated = flag;
        }
        int Get_isAuthenticated()
        {
            return isAuthenticated;
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
        std::string get_nickname() 
        {
            return _nickname;}
        void set_nickname(std::string nick) 
        {_nickname = nick;}
        Client(void);
        Client(int fd);
        Client &operator=(const Client &other);
        Client(const Client& other);
        ~Client();
        std::string get_Prefix() const;
};


#endif

