#include "channel.hpp"
#include <cstddef>

bool Channel::tab_found(std::string str)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == 9)
            return true;
    }
    return false;
}

void Server::join(Client &c)
{
    std::string command = c.getlineCmd();
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }
    std::vector<std::string> cmds = new_splite(command, ' ');

    if (c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
        return;
    }
    if (cmds.size() < 2)
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "JOIN"));
        return;
    }
    if (cmds[1] == "0")
    {
        removeClientFromAllChannels(c);
        return;
    }
    
    std::vector<std::string> key_channle;
    std::vector<std::string> multi_channel = Channel::splite_coma(cmds[1], ',');

    if (cmds.size() > 2)
        key_channle = Channel::splite_coma(cmds[2], ',');



    for(size_t i = 0; i < multi_channel.size() ; i++)
    {
        std::string one_channel = multi_channel[i];
        std::string one_channel1 = multi_channel[i];
        std::string key = "";
        one_channel1 = Channel::to_lower(one_channel1);
        if (i < key_channle.size())
            key = key_channle[i];
        if (one_channel.empty() || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || Channel::tab_found(one_channel) == true)
        {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
            continue;
        }
        std::map<std::string, Channel>::iterator it = channel.find(one_channel1);
        if (it == channel.end())
        {
            Channel new_ch(one_channel1);
            channel.insert(std::make_pair(one_channel1, new_ch));
            Channel &join =  channel[one_channel1];
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
        else
        {
            Channel &join = it->second;
            std::string topic = join.getTopic();
            bool  is_invited = join.isInvited(c);
            if (join.Check_mode('i') == true && !is_invited)
                sendReply(c, error.ERR_INVITEONLYCHAN(c.get_nickname(), one_channel));
            else if (join.Check_mode('l') && join.is_full() == true && !is_invited)
                sendReply(c, error.ERR_CHANNELISFULL(c.get_nickname(), one_channel));
            else if (join.Check_mode('k') == true && key != join.Get_key() && !is_invited)
                sendReply(c, error.ERR_BADCHANNELKEY(c.get_nickname(), one_channel));
            else
            {
                if(join.isUserInChannel(c) == false)
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