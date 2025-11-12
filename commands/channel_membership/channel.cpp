#include "channel.hpp"
#include <sstream>

Channel::Channel(std::string name) : Channel_name(name)
{
    this->isKeySet = false;
    this->isInviteOnly = false;
    this->limit = false;
    this->num_limite = 0;
    this->key = "";
}
void Channel::broadcast(const std::string &msg)
{

    Server access;
    for(std::map<std::string,Client*>::iterator it = users.begin(); it != users.end();  it++)
    {
        Client *c = it->second;
        access.sendReply(*c, msg);
    }
    
}
std::string Channel::getNamesList()
{
    std::string  name = "";
    for(std::map<std::string, Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if(operators_.find(it->first) != operators_.end())
            name  = name  + '@';
        name += it->second->get_nickname();
        name  += " ";
    }
    if(!name.empty())
        name.erase(name.length() - 1, 1);
    return name;
}

void Channel::Add_to_admin(Client &c)
{
   operators_.insert(std::make_pair(c.get_nickname(),&c));
}


void Channel::Add_to_user(Client &c)
{
   users.insert(std::make_pair(c.get_nickname(),&c));
}


void Channel::Add_to_invite(Client &c)
{
   invite.insert(std::make_pair(c.get_nickname(),&c));
}

std::string Channel::Get_key()
{
    return this->key;
}

bool Channel::isClientOperator(Client &c) const
{
    return (operators_.find(c.get_nickname()) != operators_.end()); 
}


bool Channel::isClientUSER(Client &c) const
{
    return (operators_.find(c.get_nickname()) != operators_.end()); 
}

std::string Channel::Set_key()
{
    return this->key;
}
bool Channel::isInvited(Client &c)
{
    return (invite.find(c.get_nickname()) != invite.end());
}
bool Channel::is_full()
{
    return(users.size() >=  get_num_limite());
}

void Channel::rm_user_from_channel(Client &c)
{
    std::map<std::string, Client*>::iterator it_user = users.find(c.get_nickname());
    if(it_user != users.end())
    {
        users.erase(c.get_nickname());
    }
    std::map<std::string, Client*>::iterator it_operator = operators_.find(c.get_nickname());
    if(it_operator != operators_.end())
    {
        operators_.erase(c.get_nickname());
    }
}

bool Channel::Check_mode(char mode)
{
    if(mode == 'k')
        return this->isKeySet;
    else if(mode == 'i')
        return this->isInviteOnly;
    else if(mode == 'l')
        return this->limit;
    return false;
}


bool  Channel::isEmpty()
{
    if(users.size() == 0)
        return true;
    return false;
}


bool Channel::isUserInChannel(Client &c)
{
    return(users.find(c.get_nickname()) != users.end());
}

std::vector<std::string> Channel::splite_coma(std::string &strr, char d)
{
    std::string save;
    std::stringstream ss(strr);
    std::vector<std::string> resulte;

    while(getline(ss,  save , d))
    {
            resulte.push_back(save);
    }
    return resulte;
}

std::string Channel::to_lower(std::string str)
{
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

void Channel::setTopic( std::string topic ) {
    this->channelTopic = topic;
}

std::string Channel::getTopic() {
    return this->channelTopic;
}