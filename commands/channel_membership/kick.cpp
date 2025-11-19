#include "../../server/Server.hpp"
#include "channel.hpp"

void Server::kick(Client &c)
{
    std::string command = c.getlineCmd();
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }
    std::vector<std::string> cmds = new_splite(command, ' ');

    if (c.Get_isAuthenticated() == false)
        return(sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname())));
    if (cmds.size() < 3)
        return(sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "KICK")));

    std::string one_channel = cmds[1];
    std::string name_c_to_kick = cmds[2];
    std::string reason = "";

    if (cmds.size() > 3)
    {
        std::vector<std::string>::iterator it = cmds.begin() + 3;
        if (it->size() > 0 && (*it)[0] == ':')
            reason += it->substr(1);
        else
            reason += *it;
        it++;
        for (; it != cmds.end(); it++)
        {
            reason += " "; 
            reason += *it;
        }
    }
    Client *client_to_kick = find_client_by_nickname(name_c_to_kick);
    if (!client_to_kick)
        return(sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), name_c_to_kick)));
    std::map<std::string, Channel>::iterator it = channel.find(one_channel);
    if ((one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || it == channel.end())
        return(sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel)));
    else if (!it->second.isUserInChannel(c))
        return(sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(), one_channel)));
    else if (!it->second.isUserInChannel(*client_to_kick))
        return sendReply(c, error.ERR_USERNOTINCHANNEL(c.get_nickname(), name_c_to_kick, one_channel));
    else if (it->second.isClientOperator(c) == false)
       return sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel));
    else 
    {
        it->second.broadcast(error.MSG_KICK(c.get_Prefix(), it->second.get_channel_name(), name_c_to_kick, reason));
        it->second.removeClientFromOneChannels(*client_to_kick);
    }
}