/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:05:12 by akella            #+#    #+#             */
/*   Updated: 2025/11/30 20:37:46 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include <cctype>
#include <cstddef>


bool Channel::whitespace_found(std::string str)
{
    for(size_t i = 0; i <= str.size(); i++)
    {
        if(std::isspace(str[i]))
            return true;
    }
    return false;
}

void Server::join(Client &c)
{
    
    std::string command = c.getlineCmd();
    
    if (!command.empty() && command[command.size() -1 ] == '\n')
        command.erase(command.size() -1);

    std::vector<std::string> cmds = new_splite(command, ' ');
    
    if (c.Get_isAuthenticated() == false)
        return sendReply(c, error.ERR_NOT_REGESTRED());
    
    if (cmds.size() < 2)
        return sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "JOIN"));
    
    if (cmds[1] == "0")
        return removeClientFromAllChannels(c);
    
    std::vector<std::string> key_channle;
    std::vector<std::string> multi_channel = Channel::split_comma(cmds[1], ',');

    if (cmds.size() > 2)
        key_channle = Channel::split_comma(cmds[2], ',');

    for (size_t i = 0; i < multi_channel.size(); i++)
    {
        std::string one_channel = multi_channel[i];
        std::string one_channel1 = multi_channel[i];
        std::string key = "";
        
        one_channel1 = Channel::to_lower(one_channel1);

        if (i < key_channle.size())
            key = key_channle[i];
        if (one_channel.empty())
            continue;
        if (one_channel == "0")
        {
            removeClientFromAllChannels(c);
            continue;
        }
        if (one_channel.empty() || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || Channel::whitespace_found(one_channel) == true)
        {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
            continue;
        }

        std::map<std::string, Channel>::iterator it = channel.find(one_channel1);
        
        if (it == channel.end())
        {
            Channel new_ch(one_channel1);
            channel.insert(std::make_pair(one_channel1, new_ch));
            Channel &join = channel[one_channel1];
            std::string topic = join.getTopic();

            join.Add_to_admin(c);
            join.Add_to_user(c);
            join.broadcast(error.MSG_JOIN(c.get_Prefix(), one_channel));
            sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join.getNamesList()));
            sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
        }
        else 
        {
            Channel &join = it->second;
            std::string topic = join.getTopic();
            bool is_invited = join.isInvited(c);
            if (join.isUserInChannel(c) == false) 
            {
            if (join.Check_mode('i') == true && !is_invited)
                sendReply(c, error.ERR_INVITEONLYCHAN(c.get_nickname(), one_channel));
            else if (join.Check_mode('l') && join.is_full() == true && !is_invited)
                sendReply(c, error.ERR_CHANNELISFULL(c.get_nickname(), one_channel));
            else if (join.Check_mode('k') == true && key != join.Get_key() && !is_invited)
                sendReply(c, error.ERR_BADCHANNELKEY(c.get_nickname(), one_channel));
            else
            {
                    join.Add_to_user(c);
                    join.broadcast(error.MSG_JOIN(c.get_Prefix(), one_channel));

                    if (topic.empty())
                        sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
                    else
                    {
                        sendReply(c, error.RPL_TOPIC(c.get_nickname(), one_channel, topic));
                        sendReply(c, error. RPL_TOPICWHOTIME (c.get_nickname(), it->second.fromTime(it->second.getTimeTopic()), it->second.get_channel_name(), it->second.getTopicChanger()));
                    }

                    sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join.getNamesList()));
                    sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
                }
            }
        }
    }
}