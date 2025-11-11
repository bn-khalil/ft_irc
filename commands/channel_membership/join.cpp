#include "channel.hpp"

void Server::join(std::vector<std::string> cmds, Client &c)
{
    // exit(1);
    // std::cout << cmds[1] << std::endl;
    // exit(1);
    // c.set_nickname("default");

    if(cmds.size() < 2)
    {
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "JOIN", "<channel>[,<channel>]+ [<key>[,<key>]+]"));
        return ;
    }
  if(cmds[1] == "0")
    {
        std::cout << "testesttesttest" << std::endl;
        removeClientFromAllChannels(c);
        // return ;
    }
    std::vector<std::string> key_channle;
    std::vector<std::string> multi_channel = access.splite_coma(cmds[1], ',');

    if(cmds.size() > 2)
        key_channle = access.splite_coma(cmds[2], ',');


  
    size_t i = 0;

    while(i < multi_channel.size())
    {
            std::string one_channel = multi_channel[i];
            std::string key = "";
            one_channel = access.to_lower(one_channel);
            if(i < key_channle.size())
                key  = key_channle[i];

    if(one_channel.length() < 2 || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200)
    {
        sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
        i++;
        continue ;
    }

    Channel *join;

    std::map<std::string, Channel *>::iterator it = channel.find(one_channel);
    if(it == channel.end())
    {
        join = new Channel(one_channel);
        channel[one_channel] = join;
        join->Add_to_admin(c);
        join->Add_to_user(c);
        // broadcast( error.MSG_JOIN(c.get_Prefix(), one_channel) );
        sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
        sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join->getNamesList()));
        sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
    }
    else
    {
        join = it->second;
        if(join->Check_mode('l') && join->is_full() == true)
            sendReply(c, error.ERR_CHANNELISFULL(c.get_nickname(), one_channel));
        else if(join->Check_mode('i') == true && !join->isInvited(c))
            sendReply(c, error.ERR_INVITEONLYCHAN(c.get_nickname(), one_channel));
        else if(join->Check_mode('k') == true && key != join->Get_key())
            sendReply(c, error.ERR_BADCHANNELKEY(c.get_nickname(), one_channel));
        else
        {
            join->Add_to_user(c);
        // broadcast( error.MSG_JOIN(c.get_Prefix(), one_channel) );
        // sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), one_channel));
        sendReply(c, error.RPL_NAMREPLY(c.get_nickname(), one_channel, join->getNamesList()));
        sendReply(c, error.RPL_ENDOFNAMES(c.get_nickname(), one_channel));
        }
    }
        i++;
    }
}
