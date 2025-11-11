#include <cstddef>
#include <sstream>
#include <vector>
#include "../../server/Client.hpp"
#include "../../server/Server.hpp"
#include "Reply.hpp"
#include "channel.hpp"
#include "Reply.hpp"
#include <sys/socket.h>
#include "../../server/Client.hpp"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <utility>
#include <map>

//JOIN 0                          ; Leave all currently joined
                                  // channels.
//splite by comma
//check
//brodcast
// void Channel::sendReply(Client *c, std::string msg)
// {
//     std::string full_msg = msg + "aaaa" + "\r\n";
//     (void)c;
//     std::cout << send(4 , full_msg.c_str() , full_msg.length(), 0)  << "\n";
//     // if(send(4 , full_msg.c_str() , full_msg.length(), 0) )
//     // {
//     //     std::cerr << "Client Disconnected" << std::endl;
//     // }
// }
Channel::Channel(std::string name) : Channel_name(name)
{
    this->isKeySet = false;
    this->isInviteOnly = false;
    this->limit = false;
    this->num_limite = 0;
    this->key = "";
}
std::string Channel::getNamesList()
{
    std::string  name = "";
    for(std::map<std::string, Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if(operators_.find(it->first) != operators_.end())
            name  = name  + '@';
        name += it->first;
        name  += " ";
    }
    if(!name.empty())
        name.erase(name.length() - 1, 1);
    return name;
}


void Server::broadcast(const std::string &msg)
{

    std::map<std::string,Client*> op = chan->get_operators_();
    std::map<std::string,Client*> us = chan->get_users();
    for(std::map<std::string,Client*>::iterator it = op.begin(); it != us.end();  it++)
    {
        Client *c = it->second;
        sendReply(c, msg);
    }
    
}
void Channel::Add_to_admin(Client *c)
{
   operators_.insert(std::make_pair(c->get_nickname(),c));
}


void Channel::Add_to_user(Client *c)
{
   users.insert(std::make_pair(c->get_nickname(),c));
}


void Channel::Add_to_invite(Client *c)
{
   invite.insert(std::make_pair(c->get_nickname(),c));
}

std::string Channel::Get_key()
{
    return this->key;
}

bool Channel::isClientOperator(Client *c) const
{
    return (operators_.find(c->get_nickname()) != operators_.end()); 
}
bool Channel::isClientUSER(Client *c) const
{
    return (operators_.find(c->get_nickname()) != operators_.end()); 
}


std::string Channel::Set_key()
{
    return this->key;
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

bool Channel::isInvited(Client *c)
{
    return (invite.find(c->get_nickname()) != invite.end());
}
bool Channel::is_full()
{
    return(users.size() >=  get_num_limite());
}

void Channel::rm_user_from_channel(Client *c)
{
    // Channel *chann;
    std::map<std::string, Client*>::iterator it_user = users.find(c->get_nickname());
    if(it_user != users.end())
    {
        users.erase(c->get_nickname());
    }
    std::map<std::string, Client*>::iterator it_operator = operators_.find(c->get_nickname());
    if(it_operator != operators_.end())
    {
        operators_.erase(c->get_nickname());
    }
}

bool  Channel::isEmpty()
{
    if(users.size() == 0)
        return true;
    return false;
}
void Server::removeClientFromAllChannels(Client *c)
{
    std::vector<std::string> channel_to_leave;
    std::map<std::string, Channel *>::iterator it = this->channel.begin();
    while(it != this->channel.end())
    {
        Channel *ch = it->second;
        if(ch->isUserInChannel(c))
            channel_to_leave.push_back(ch->get_channel_name());
        it++;
    }
    // std::map<std::string,Channel*>::iterator it1 = channel.find(channel_to_leave[i]);
    // while(i < channel_to_leave.size())
    // {
    // }
    for(size_t i = 0; i < channel_to_leave.size();i++)
    {
        std::map<std::string,Channel*>::iterator it1 = channel.find(channel_to_leave[i]);
        if(it1 == channel.end())
            continue;
        Channel *chan = it1->second;
       // brodcast to other user;
       broadcast( error.MSG_PART(c->get_Prefix(), chan->get_channel_name(), "Leaving") );
       // rm user from channel;
        chan->rm_user_from_channel(c);
       // rm channel if no one left  
       if(chan->isEmpty() == true)
       {
            channel.erase(it1);
            delete chan;
       }
    }
    
}

bool Channel::isUserInChannel(Client *c)
{
    return(users.find(c->get_nickname()) != users.end());
}
void Server::sendReply(Client *c, std::string msg)
{
    
    std::string full_msg = msg + "\r\n";
    (void) c;
    if(send(4 , full_msg.c_str() , full_msg.length(), 0) <= -1) //lenght and size
    {
        std::cerr << "Client Disconnected" << std::endl;
    }
    
}
// void Server::quite_from_all_channel(Client *c, Channel *ch)
// {
//     std::string nickname = c->nickname;
//     std::map<std::string, Client*>::iterator it = ch->users.find(nickname);
//     if(it == ch->users.end())
//         return ;
//     else
//         users.erase(nickname);
// }
std::vector<std::string> splite_coma(std::string &strr, char d)
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
std::string to_lower(const std::string& str)
{
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}
void Server::join(std::vector<std::string> cmds, Client *c)
{
    if(cmds.size() < 2)
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c->get_nickname(), "JOIN"));
        return ;
    }
    // if(cmds[1] == "0")
    // {
    //     quite_from_all_channel(c);
    // }

    std::vector<std::string> key_channle;
    std::vector<std::string> multi_channel = splite_coma(cmds[1], ',');

    if(cmds.size() > 2)
        key_channle = splite_coma(cmds[2], ',');

    if(cmds[1] == "0")
        removeClientFromAllChannels(c);
    size_t i = 0;

    while(i < multi_channel.size())
    {
            std::string one_channel = multi_channel[i];
            std::string key = "";
            one_channel = to_lower(one_channel);
            if(i < key_channle.size())
                key  = key_channle[i];
    if(one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200)
    {
        sendReply(c, error.ERR_NOSUCHCHANNEL(c->get_nickname(), one_channel));
        i++;
        continue ;
    }

    Channel *join;

    std::map<std::string, Channel *>::iterator it = channel.find(one_channel);
    if(it == channel.end())
    {
        join = new Channel(one_channel);
        channel[one_channel] = join;
        join->Add_to_admin(c);
        join->Add_to_user(c);
        // broadcast( error.MSG_JOIN(c->get_Prefix(), one_channel) );
        sendReply(c, error.RPL_NOTOPIC(c->get_nickname(), one_channel));
        sendReply(c, error.RPL_NAMREPLY(c->get_nickname(), one_channel, join->getNamesList()));
        sendReply(c, error.RPL_ENDOFNAMES(c->get_nickname(), one_channel));
    }
    else
    {
        join = it->second;
        if(join->Check_mode('l') && join->is_full() == true)
            sendReply(c, error.ERR_CHANNELISFULL(c->get_nickname(), one_channel));
        else if(join->Check_mode('i') == true && !join->isInvited(c))
            sendReply(c, error.ERR_INVITEONLYCHAN(c->get_nickname(), one_channel));
        else if(join->Check_mode('k') == true && key != join->Get_key())
            sendReply(c, error.ERR_BADCHANNELKEY(c->get_nickname(), one_channel));
        else
        {
            join->Add_to_user(c);
        //send msg to all user we have new user 
        // succsec  one !!
        broadcast( error.MSG_JOIN(c->get_Prefix(), one_channel) );
        sendReply(c, error.RPL_NOTOPIC(c->get_nickname(), one_channel));
        sendReply(c, error.RPL_NAMREPLY(c->get_nickname(), one_channel, join->getNamesList()));
        sendReply(c, error.RPL_ENDOFNAMES(c->get_nickname(), one_channel));

        }
    }
        i++;
    }
}
