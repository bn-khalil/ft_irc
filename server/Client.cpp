#include "Client.hpp"

Client::Client(void)
{

}

Client::Client(int fd)
{
    this->fd = fd;
    isSetPass=false;
    isSetNick=false;
    isAuthenticated = false;
    isSetuser = false;
    _nickname= "*" ;
    _username= "" ;
    _hostname= "" ;
}

Client::Client(const Client &other)
{
    (void) other;

}

std::string Client::    get_Prefix() const
{
    std::cout << this->_nickname << std::endl;
    std::cout << this->_username << std::endl;
    std::cout << "hostname" << this->_hostname<< std::endl;
	return  this->_nickname + "!~" + this->_username + "@" + this->_hostname;
}


 
Client::~Client(void)
{
}