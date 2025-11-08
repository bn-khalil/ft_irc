#include "Client.hpp"

Client::Client(void)
{

}

Client::Client(int fd)
{
    this->fd = fd;
    isAuthenticated = false;
    _nickname= "" ;
    _username= "" ;
}

Client::Client(const Client &other)
{
    (void) other;

}

Client &Client::operator=(const Client &other)
{
    if (&other != this)
    {

    }
    return (*this);
}

 
Client::~Client(void)
{
}

