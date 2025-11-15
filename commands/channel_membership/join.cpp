#include "channel.hpp"

bool tab_found(std::string str)
{
    size_t i = 0;
    while (i < str.size())
    {
        if (str[i] == 9)
            return true;
        i++;
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

    size_t i = 0;

    while (i < multi_channel.size())
    {
        std::string one_channel = multi_channel[i];
        std::string key = "";
        one_channel = Channel::to_lower(one_channel);
        
        if (i < key_channle.size())
            key = key_channle[i];
        //CHECK IF ALWAYS THE NAME LOWER OR OPERCASE
        // handl to many channel
        if (one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || tab_found(one_channel) == true)
        {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
            i++;
            continue;
        }

        Channel *join;
        
        std::map<std::string, Channel *>::iterator it = channel.find(one_channel);
        if (it == channel.end())
        {
            join = new Channel(one_channel);
            channel[one_channel] = join;
            join->Add_to_admin(c);
            join->Add_to_user(c);
            join->broadcast(error.MSG_JOIN(c.get_Prefix(), one_channel));
            if (join->getTopic() == "")
                    sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
            if (join->getTopic() != "")
                    sendReply(c, error.RPL_TOPIC(c.get_nickname(), one_channel, join->getTopic()));
            sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join->getNamesList()));
            sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
        }
        else
        {
            join = it->second;
            if (join->Check_mode('l') && join->is_full() == true)
                sendReply(c, error.ERR_CHANNELISFULL(c.get_nickname(), one_channel));
            else if (join->Check_mode('i') == true && !join->isInvited(c))
                sendReply(c, error.ERR_INVITEONLYCHAN(c.get_nickname(), one_channel));
            else if (join->Check_mode('k') == true && key != join->Get_key())
                sendReply(c, error.ERR_BADCHANNELKEY(c.get_nickname(), one_channel));
            else
            {
                if(!join->isUserInChannel(c))  
                {
                join->Add_to_user(c);
                join->broadcast(error.MSG_JOIN(c.get_Prefix(), one_channel));
                if (join->getTopic() == "")
                    sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
                if (join->getTopic() != "")
                    sendReply(c, error.RPL_TOPIC(c.get_nickname(), one_channel, join->getTopic()));
                sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join->getNamesList()));
                sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
                }
            }
        }
        i++;
    }
}d