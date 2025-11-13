#include "../../server/Server.hpp"
#include "channel.hpp"


void  Server::invit(std::vector<std::string> cmds, Client &c)
{  
    if(cmds.size() >= 3)
    {
        std::string one_channel = cmds[2];
        std::string name_c_invited = cmds[1];
        Client *client_invited = chan->search_nick_of_inveted(name_c_invited);
        if(!client_invited)
        {
            std::cout << "401 nicknick user :No such nick" << std::endl;
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
            error.ERR_NOTONCHANNEL(c.get_nickname(), one_channel);
            return ;

        }

        if(chan->isClientOperator(c) == true)
        {
            error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel);
            return ;
        }
        chan->Add_to_invite(*client_invited);
        chan->broadcast("CHECK_REFERANCE_MSG");
    }
    else 
    {
        std::cout << "print channel list " << std::endl;
    }
}
