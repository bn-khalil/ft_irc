#include "Client.hpp"

Client::Client()
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


std::string Client::    get_Prefix() const
{
    std::cout << this->_nickname << std::endl;
    std::cout << this->_username << std::endl;
    std::cout << "hostname" << this->_hostname<< std::endl;
	return  this->_nickname + "!~" + this->_username + "@" + this->_hostname;
}

    std::string Client::get_username()
    {
        return _username;
    }

    std::string Client::get_realname()
    {
        return _realname;
    }

    void Client::set_username(std::string _username)
    {
        this->_username = _username;
    }

    void Client::set_realname(std::string _realname)
    {
        this->_realname = _realname;
    }

    void Client::setlineCmd( std::string &line)
    {
        lineCmd = line;
    }

    std::string& Client::getlineCmd()
    {
        return lineCmd;
    }

    void Client::setfd( int fd)
    {
        this->fd = fd;
    }

    int Client::getfd()
    {
        return this->fd;
    }

    void Client::Set_isAuthenticated( bool flag)
    {
        isAuthenticated = flag;
    }

    int Client::Get_isAuthenticated()
    {
        return isAuthenticated;
    }

    void Client::SetIsSetPass(bool flag)
    {
        isSetPass = flag ;
    }

    bool Client::GetIsSetPass(){
        return isSetPass;
    }

    void Client::SetIsSetNick(bool flag)
    {
        isSetNick= flag ;
    }

    bool Client::GetIsSetNick(){
        return isSetNick;
    }

    void Client::SetIsSetuser(bool flag)
    {
        isSetuser= flag ;
    }

    bool Client::GetIsSetuser(){
        return isSetuser;
    }

    std::string Client::get_nickname() 
    {
        return _nickname;
    }

    void Client::set_nickname(std::string nick) 
    {
        _nickname = nick;
    }

    void Client::set_hostname(std::string hostname){
        _hostname = hostname;
    }

    std::string Client::get_hostname()
    {
        return _hostname;
    }


 
Client::~Client(void)
{
}


// :bn!~SD@freenode-obu.d75.6g0qj4.IP TOPIC #b :new