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
        std::string _realname;
        std::string _hostname;
        std::string lineCmd;
        bool isSetPass;
        bool isSetNick;
        bool isSetuser;

    public:
        std::string get_username()
        {
            return _username;
        }
        std::string get_realname()
        {
            return _realname;
        }
        void set_username(std::string _username)
        {
            this->_username = _username;
        }
        void set_realname(std::string _realname)
        {
            this->_realname = _realname;
        }
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
        void SetIsSetuser(bool flag)
        {
            isSetuser= flag ;
        }
        bool GetIsSetuser(){
            return isSetuser;
        }
        std::string get_nickname() 
        {
            return _nickname;
        }

        void set_nickname(std::string nick) 
        {
            _nickname = nick;
        }

        Client(void);
        Client(int fd);
        Client(const Client& other);
        ~Client();
        void set_hostname(std::string hostname){
            _hostname = hostname;
        }
        std::string get_hostname()
        {
            return _hostname;
        }
        std::string get_Prefix() const;
};  


#endif

