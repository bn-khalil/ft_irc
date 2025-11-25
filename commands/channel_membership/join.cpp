/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:05:12 by akella            #+#    #+#             */
/*   Updated: 2025/11/25 18:24:54 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include <cstddef>

//Check If The Name Of Channel Have Tab !!
bool Channel::tab_found(std::string str)
{
    return str.find(9) != std::string::npos;
}

void Server::join(Client &c)
{
    //Split The String Using Just Space !!
    std::string command = c.getlineCmd();
    
    if (!command.empty() && command[command.size() -1 ] == '\n')
        command.erase(command.size() -1);

    std::vector<std::string> cmds = new_splite(command, ' ');
    //Check if Authenticated Alreddy Done !!
    if (c.Get_isAuthenticated() == false)
        return sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
    //If Size Less than 2 ERROR !!
    if (cmds.size() < 2)
        return sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "JOIN"));
    // 0 To Remove Client From All Channel !!
    if (cmds[1] == "0")
        return removeClientFromAllChannels(c);
    //Splite The Channel In Case join #name1,#name2   Key1,Key2 or More !!
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

        //Parse The Name Most be Valide !! 
        if (one_channel.empty() || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || Channel::tab_found(one_channel) == true)
        {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
            continue;
        }

        std::map<std::string, Channel>::iterator it = channel.find(one_channel1);
        //If The Channel Not found We Need to Creat One  And Add USER to Operator And Users!!
        if (it == channel.end())
        {
            Channel new_ch(one_channel1);
            channel.insert(std::make_pair(one_channel1, new_ch));
            Channel &join = channel[one_channel1];
            std::string topic = join.getTopic();

            join.Add_to_admin(c);
            join.Add_to_user(c);
            join.broadcast(error.MSG_JOIN(c.get_Prefix(), one_channel));

            if (topic.empty())
                sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
            else
                sendReply(c, error.RPL_TOPIC(c.get_nickname(), one_channel, topic));

            sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join.getNamesList()));
            sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
        }
        else // If The Channel Alreaddy Created We Take It And  Add the user just to users After Check The Modes Ofcourse !!
        {
            Channel &join = it->second;
            std::string topic = join.getTopic();
            bool is_invited = join.isInvited(c);
            if (join.isUserInChannel(c) == false) // Brodcast The msg Only if The user  not in the Channel !!
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
                        sendReply(c, error.RPL_TOPIC(c.get_nickname(), one_channel, topic));

                    sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join.getNamesList()));
                    sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
                }
            }
        }
    }
}