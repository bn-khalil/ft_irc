#include "../../server/Server.hpp"
#include "channel.hpp"


void  Server::invit(std::vector<std::string> cmds, Client &c)
{
    // (void)cmds;
    // (void)c;
//  Channel access("accsess");
    
    if(cmds.size() < 2)
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "INVITE", "<channel>[,<channel>]+ [<key>[,<key>]+]"));
        return ;
    }

    std::string one_channel = cmds[1];
    if(one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200)
    {
        sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
        return ;
    }
    std::map<std::string , Channel*>::iterator it = channel.find(one_channel);
    if(it == channel.end())
    {
        error.ERR_NOTONCHANNEL(c.get_nickname(), one_channel);
        return ;

    }
    std::map<std::string, Client*> op = access.get_operators_();

    std::map<std::string , Client*>::iterator it1 = op.find(c.get_nickname());
    if(it1 == op.end())
    {
        error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel);
        return ;
    }
    std::map<std::string, Client*>  us = access.get_users();

    std::map<std::string , Client*>::iterator it2 = us.find(c.get_nickname());
    if(it2 == us.end())
    {
        error.ERR_USERNOTINCHANNEL(c.get_nickname(), cmds[1] ,one_channel);
        return ;
    }
    access.Add_to_invite(c);
    access.broadcast("CHECK_REFERANCE_MSG");
return ;

}
