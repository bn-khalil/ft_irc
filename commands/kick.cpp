/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kben-tou <kben-tou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:05:07 by akella            #+#    #+#             */
/*   Updated: 2025/11/29 18:26:18 by kben-tou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    {
        return sendReply(c, error.ERR_NOT_REGESTRED());
    }
    if (cmds.size() < 3)
    {
        return sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "KICK"));
    }
    std::string reason = handle_the_resone(cmds, command, c);

    if (!find_client_by_nickname(cmds[2]))
    {
        return sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), cmds[2]));
    }
    std::map<std::string, Channel>::iterator it = channel.find(toLower(cmds[1]));
    if (it == channel.end() || name_perfect(cmds))
    {
        return sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), cmds[1]));
    }
    else if (!it->second.isUserInChannel(c))
    {
        return sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(), cmds[1]));
    }
    else if (!it->second.isUserInChannel(*find_client_by_nickname(cmds[2])))
    {
        return sendReply(c, error.ERR_USERNOTINCHANNEL(c.get_nickname(), cmds[2], cmds[1]));
    }
    
    else if (it->second.isClientOperator(c) == false)
    {
        return sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), cmds[1]));
    }
    else
    {
        kick_with_brodcast(it, cmds, reason, c);
    }
}