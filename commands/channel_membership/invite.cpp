#include "../../server/Server.hpp"
#include "channel.hpp"


void  Server::invit(std::vector<std::string> cmds, Client &c)
{  
    // if(c.Get_isAuthenticated() == false)
    // {
    //     sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
    //     return ;
    // }
    if(cmds.size() >= 3)
    {
        std::string one_channel = cmds[2];
        std::string name_c_invited = cmds[1];
        Client *client_invited = find_client_by_nickname(name_c_invited);
        if(!client_invited)
        {
            std::cout << error.ERR_NOSUCHNICK(c.get_nickname(), name_c_invited);
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
        real_one->broadcast(error.RPL_INVITING(c.get_nickname(), name_c_invited, one_channel));
    }
    else if(cmds.size() == 1)
    {
        for(std::map<std::string,Channel*>::iterator it = channel.begin();  it != channel.end() ; it++)
        {
            Channel *ch = it->second;
            if(ch->isUserInChannel(c))
            {
                sendReply(c, error.RPL_INVITELIST(c.get_nickname(), ch->get_channel_name()));
            }
        }
        sendReply(c, error.RPL_ENDOFINVITELIST(c.get_nickname()));
        
    }
    else 
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "INVITE"));
    }
}
