#include <cstdio>
#include <cstring>
#include <sstream>
#include <system_error>
#include <vector>
#include "../../server/Client.hpp"
#include "../../server/Server.hpp"
#include "channel.hpp"
#include <sys/socket.h>

//JOIN 0                          ; Leave all currently joined
                                  // channels.
//splite by comma
//check


bool Channel::isUserInChannel(Client *c)
{
    return(users.find(c->nickname) != users.end());
}
void sendReply(Client *c, std::string msg)
{
    
    std::string full_msg = msg + "\r\n";
    if(send(c->fd_Sock , full_msg.c_str() , full_msg.length(), 0) <= 0)
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
void Server::join(std::vector<std::string> cmds, Client *c)
{
    if(cmds.size() < 2)
    {
        sendReply(c, ERR_NEEDMOREPARAMS(c->nickname, "JOIN"));
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

    int i = 0;

    while(i < multi_channel.size())
    {
            std::string one_channel = multi_channel[i];
            std::string key = "";

            if(i < key_channle.size())
                key  = key_channle[i];

    if(one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200)
    {
        sendReply(c, ERR_NOSUCHCHANNEL(c->nickname, one_channel));
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
    }
    else
    {
        join = it->second;
        if(join->Check_mode('l') && join->is_full() == true)
            sendReply(c, ERR_CHANNELISFULL(c->nickname, one_channel));
        else if(join->Check_mode('i') == true && !join->isInvited(c))
            sendReply(c, ERR_INVITEONLYCHAN(c->nickname, one_channel));
        else if(join->Check_mode('k') == true && key != join->Get_key())
            sendReply(c, ERR_BADCHANNELKEY(c->nickname, one_channel));
        else
            join->Add_to_user(c);
    }
        i++;
    }
}
