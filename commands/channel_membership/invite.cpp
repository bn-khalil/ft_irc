#include "../../server/Server.hpp"
#include "channel.hpp"
#include <cstddef>

void  Server::invit(std::vector<std::string> cmds, Client &c)
{  
    if(c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
        return ;
    }
    if(cmds.size() > 2)
    {
        std::string one_channel = cmds[2];
        std::string name_c_invited = cmds[1];
        Client *client_invited = find_client_by_nickname(name_c_invited);
        if(!client_invited)
        {
            sendReply(c,error.ERR_NOSUCHNICK(c.get_nickname(), name_c_invited));
            return ;
        }
    
        std::map<std::string , Channel*>::iterator it = channel.find(one_channel);
        if(one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || it == channel.end())
        {
             sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
         return ;
        }
        Channel *real_one = it->second;
        
        if(real_one->isUserInChannel(*client_invited))
        {
            sendReply(c, error.ERR_USERONCHANNEL(c.get_nickname() ,name_c_invited, one_channel));
            return ;
        }

        if(real_one->isClientOperator(c) == false)
        {
            sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel));
            return ;
        }
        real_one->Add_to_invite(*client_invited);
        sendReply(c, error.RPL_INVITING(c.get_nickname(), name_c_invited, one_channel));
        sendReply(*client_invited, error.MSG_INVITE(c.get_Prefix(), name_c_invited, one_channel));
    }
    else if(cmds.size() == 1)
    {
        std::vector<std::string> all_invited_channel = isUserInvited_to_channel(c);
        size_t i = 0;
        while(i < all_invited_channel.size())
        {
            sendReply(c, error.RPL_INVITELIST(c.get_nickname(), all_invited_channel[i]));
            i++;
        }
        sendReply(c, error.RPL_ENDOFINVITELIST(c.get_nickname()));
        
    }
    else 
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "INVITE"));
    }
}
