#include "Client.hpp"

Client::Client(void)
{

}

Client::Client(int fd)
{
    this->fd = fd;
    isSetPass=false;
    isSetNick=false;
    std::cout << fd << " set with clien(fd)" << std::endl;
    isAuthenticated = false;
    _nickname= "" ;
    _username= "" ;
    _hostname= "" ;
}

Client::Client(const Client &other)
{
    (void) other;

}



std::string Client::    get_Prefix() const
{
	return ":" + this->_nickname + "!" + this->_username + "@" + this->_hostname;
}


 
Client::~Client(void)
{
}

