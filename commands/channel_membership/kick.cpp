/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:05:07 by akella            #+#    #+#             */
/*   Updated: 2025/11/27 10:24:54 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../server/Server.hpp"
#include "channel.hpp"

void Server::kick(Client &c)
{
    std::string command = c.getlineCmd();
    
    if (!command.empty() && command[command.size() -1 ] == '\n')
    {
        command.erase(command.size() -1);
    }

    std::vector<std::string> cmds = new_splite(command, ' ');

    if (c.Get_isAuthenticated() == false)
        return sendReply(c, error.ERR_NOT_REGESTRED());

    if (cmds.size() < 3)
        return sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "KICK"));

    std::string one_channel = cmds[1];
    std::string name_c_to_kick = cmds[2];
    std::string reason = "";

    if (cmds.size() > 3)
    {
        if (cmds[3][0] == ':')
        {
            size_t chan_pos = command.find(cmds[1]);
            size_t user_pos = command.find(cmds[2], chan_pos + cmds[1].length());
            size_t reason_pos = command.find(cmds[3], user_pos + cmds[2].length());
            
            if (reason_pos != std::string::npos)
                reason = command.substr(reason_pos + 1);
        }
        else
        {
            reason = cmds[cmds.size() - 1];
        }
    }
    else 
    {
        reason = c.get_nickname();
    }
    Client *client_to_kick = find_client_by_nickname(name_c_to_kick);

    if (!client_to_kick)
        return sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), name_c_to_kick));

    std::map<std::string, Channel>::iterator it = channel.find(one_channel);

    if (one_channel.empty() || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || Channel::tab_found(one_channel) == true  || it == channel.end())
        return sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
    else if (!it->second.isUserInChannel(c))
        return sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(), one_channel));
    else if (!it->second.isUserInChannel(*client_to_kick))
        return sendReply(c, error.ERR_USERNOTINCHANNEL(c.get_nickname(), name_c_to_kick, one_channel));
    else if (it->second.isClientOperator(c) == false)
        return sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel));
    else
    {
        it->second.broadcast(error.MSG_KICK(c.get_Prefix(), it->second.get_channel_name(), name_c_to_kick, reason));
        it->second.removeClientFromOneChannels(*client_to_kick);
        if(it->second.isEmpty())
            channel.erase(it); 
    }
}