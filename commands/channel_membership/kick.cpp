#include "../../server/Server.hpp"
#include "channel.hpp"


void Server::kick(std::vector<std::string> cmds, Client &c)
{
    if (c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
        return;
    }

    // splite with tab or space
    if (cmds.size() < 3)
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "KICK"));
        return;
    }

    std::string one_channel = cmds[1];
    std::string name_c_to_kick = cmds[2];
    std::string reason = "";

//take the full reason
    if (cmds.size() > 3)
    {
        std::vector<std::string>::iterator it = cmds.begin() + 3;
        if(it->size() > 0 && (*it)[0] == ':')
            reason += it->substr(1);
        else
            reason += *it;
        it++;
        for(;it != cmds.end(); it++)
        {
            reason += " "; 
            reason += *it;
        }
    }

    Client *client_to_kick = find_client_by_nickname(name_c_to_kick);
    if (!client_to_kick)
    {
        sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), name_c_to_kick));
        return;
    }

    std::map<std::string, Channel *>::iterator it = channel.find(one_channel);
    if (one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || it == channel.end())
    {
        sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
        return;
    }

    Channel *real_one = it->second;

    if (!real_one->isUserInChannel(c))
    {
        sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(), one_channel));
        return;
    }

    if (!real_one->isUserInChannel(*client_to_kick))
    {
        sendReply(c, error.ERR_USERNOTINCHANNEL(c.get_nickname(), name_c_to_kick, one_channel));
        return;
    }

    if (real_one->isClientOperator(c) == false)
    {
        sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel));
        return;
    }

    real_one->broadcast(error.MSG_KICK(c.get_Prefix(), real_one->get_channel_name(), name_c_to_kick, reason));
    real_one->removeClientFromOneChannels(*client_to_kick);
    sendReply(c, error.MSG_KICK(c.get_Prefix(), real_one->get_channel_name(), name_c_to_kick, reason));
}
//nick amine
// user a a a 
// user a a a a
// join ##A
// :amine!~a@0.0.0.0 JOIN :##a
// :ircserver42.com 331 amine ##a :No topic is set
// :ircserver42.com 353 amine = ##a :@amine
// :ircserver42.com 366 amine ##a :End of /NAMES list



