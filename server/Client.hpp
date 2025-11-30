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
        std::string get_username();
        std::string get_realname();
        void set_username(std::string _username);
        void set_realname(std::string _realname);
        void setlineCmd( std::string &line);
        std::string &getlineCmd();
        void setfd( int fd);
        int getfd();
        void Set_isAuthenticated( bool flag);
        int Get_isAuthenticated();
        void SetIsSetPass(bool flag);
        bool GetIsSetPass();
        void SetIsSetNick(bool flag);
        bool GetIsSetNick();
        void SetIsSetuser(bool flag);
        bool GetIsSetuser();
        std::string get_nickname() ;
;
        void set_nickname(std::string nick) ;
        void set_hostname(std::string hostname);
        std::string get_hostname();

        std::string get_Prefix() const;
        Client(int fd);
        Client();
};  


#endif

