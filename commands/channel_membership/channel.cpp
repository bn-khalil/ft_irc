#include "channel.hpp"
#include <sstream>
#include  <sys/socket.h>
#include <sys/poll.h>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>

Channel::Channel(std::string name) : Channel_name(name)
{
    this->isKeySet = false;
    this->isInviteOnly = false;
    this->limit = false;
    this->num_limite = 0;
    this->key = "";
    this->time_creat_channel = time(0);
}

#include <iostream> // <-- تأكد أن هادي الفوق

std::string Channel::getNamesList()
{
    std::string  name = "";

    // --- DEBUG: شوف شحال من واحد كاين ---
    std::cout << "========== DEBUG: getNamesList() ==========" << std::endl;
    std::cout << "DEBUG: Total users in map: " << users.size() << std::endl;
    std::cout << "DEBUG: Total operators in map: " << operators_.size() << std::endl;
    std::cout << "-------------------------------------------" << std::endl;


    for(std::map<std::string, Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        std::cout << "DEBUG: Processing user:" << std::endl;
        
        // --- DEBUG: شوف المفتاح (Key) لي هو النيكنايم ---
        std::cout << "  -> Map Key (it->first): [" << it->first << "]" << std::endl;
        
        // --- DEBUG: شوف النيكنايم لي مسجل فـ Client* ---
        std::cout << "  -> Client* Nick (it->second->get_nickname()): [" << it->second->get_nickname() << "]" << std::endl;

        // --- DEBUG: شوف واش هو أوبيراتور ---
        if(operators_.find(it->first) != operators_.end())
        {
            std::cout << "  -> Status: Is operator." << std::endl;
            name  = name  + '@';
        }
        else
        {
            std::cout << "  -> Status: Is normal user." << std::endl;
        }

        // --- هنا فين كتزيد النيكنايم ---
        name += it->first; // (هادي كتستعمل المفتاح ديال الماب)
        
        std::cout << "  -> Current name string: [" << name << "]" << std::endl; // هادي هي لي درتي نتا
        
        name  += " ";
    }

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "DEBUG: Final string before erase: [" << name << "]" << std::endl;

    if(!name.empty())
        name.erase(name.length() - 1, 1);
    
    std::cout << "DEBUG: Final string returned: [" << name << "]" << std::endl;
    std::cout << "===========================================" << std::endl;
    
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

void Channel::broadcast(const std::string &msg)
{
    std::string full_msg = msg + "\r\n";
    for(std::map<std::string,Client*>::iterator it = users.begin(); it != users.end();  it++)
    {
        Client *c = it->second;
        send(c->getfd(), full_msg.c_str(), full_msg.length(), 0);
    }
    
}