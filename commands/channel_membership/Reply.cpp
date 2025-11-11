#include "Reply.hpp"

Reply::Reply() : server_name("ircserver42.com")
{
}
std::string Reply::ERR_NEEDMOREPARAMS(std::string nick, std::string command, std::string more)
{

         std::string msg =  ":"  + server_name   + " 461 " + nick + " " + command + " :Not enough parameters\n" + 
          ":"  + server_name   + " 650 " + nick + " " + command + ":" + more ;
    
    return msg;
}
std::string Reply::ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name)
{
    return ":" + server_name + " 403 " + nick + " " + channel_name + " :No such channel";
}
std::string Reply::ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 482 " + nick + " " + channel + " :You're not channel operator";
}
std::string Reply::ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 442 " + nick + " " + channel + " :You're not on that channel";
}

//------------------------------JOIN error------------------------------------------------------
std::string Reply::ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 475 " + nick + " " + channel + " :Cannot join channel (+k)";
}
std::string Reply::ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 473 " + nick + " " + channel + " :Cannot join channel (+i)";
}
std::string Reply::ERR_CHANNELISFULL(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 471 " + nick + " " + channel + " :Cannot join channel (+l)";
}
std::string Reply::MSG_JOIN(const std::string& user_prefix, const std::string& channel_name)
{
    return ":" + user_prefix + " JOIN :" + channel_name;
}
std::string Reply::RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic)
{
    return ":" + server_name + " 332 " + nick + " " + channel + " :" + topic;
}
std::string Reply::RPL_NOTOPIC(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 331 " + nick + " " + channel + " :No topic is set";
}
std::string Reply::RPL_NAMREPLY(const std::string& nick, const std::string& channel, const std::string& names_list)
{
    return ":" + server_name + " 353 " + nick + " = " + channel + " :" + names_list;
}

std::string Reply::RPL_ENDOFNAMES(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 366 " + nick + " " + channel + " :End of /NAMES list";
}
std::string Reply::RPL_ENDOFINVIT(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 337 " + nick + " : " + channel + " :End of /NAMES list";
}
//------------------------------KICK error------------------------------------------------------

std::string Reply::ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + server_name + " 441 " + nick + " " + target_nick + " " + channel + " :They aren't on that channel";
}
std::string Reply::MSG_KICK(const std::string& kicker_prefix, const std::string& channel, const std::string& target_nick, const std::string& reason)
{
    return ":" + kicker_prefix + " KICK " + channel + " " + target_nick + " :" + reason;
}

std::string Reply::MSG_PART(const std::string& user_prefix, const std::string& channel, const std::string& reason)
{
    return ":" + user_prefix + " PART " + channel + " :" + reason;
}
//------------------------------INVIT error------------------------------------------------------
std::string Reply::ERR_USERONCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + server_name + " 443 " + nick + " " + target_nick + " " + channel + " :is already on channel";
}
std::string Reply::RPL_INVITING(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + server_name + " 341 " + nick + " " + target_nick + " " + channel;
}
std::string Reply::MSG_INVITE(const std::string& inviter_prefix, const std::string& target_nick, const std::string& channel)
{
    return ":" + inviter_prefix + " INVITE " + target_nick + " :" + channel;
}
/* ************************************************************************** */