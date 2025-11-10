#include "../../server/Server.hpp"
#include <string>

//------------------------------General error------------------------------------------------------
std::string Server::ERR_NEEDMOREPARAMS(const std::string& nick, const std::string& command)
{
    return ":" + _serverName + " 461 " + nick + " " + command + " :Not enough parameters";
}
std::string Server::ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name)
{
    return ":" + _serverName + " 403 " + nick + " " + channel_name + " :No such channel";
}
std::string Server::ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 482 " + nick + " " + channel + " :You're not channel operator";
}
std::string Server::ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 442 " + nick + " " + channel + " :You're not on that channel";
}




//------------------------------JOIN error------------------------------------------------------
std::string Server::ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 475 " + nick + " " + channel + " :Cannot join channel (+k)";
}
std::string Server::ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 473 " + nick + " " + channel + " :Cannot join channel (+i)";
}
std::string Server::ERR_CHANNELISFULL(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 471 " + nick + " " + channel + " :Cannot join channel (+l)";
}
std::string Server::MSG_JOIN(const std::string& user_prefix, const std::string& channel_name)
{
    return ":" + user_prefix + " JOIN :" + channel_name;
}
std::string Server::RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic)
{
    return ":" + _serverName + " 332 " + nick + " " + channel + " :" + topic;
}
std::string Server::RPL_NOTOPIC(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 331 " + nick + " " + channel + " :No topic is set";
}
std::string Server::RPL_NAMREPLY(const std::string& nick, const std::string& channel, const std::string& names_list)
{
    return ":" + _serverName + " 353 " + nick + " = " + channel + " :" + names_list;
}
std::string Server::RPL_ENDOFNAMES(const std::string& nick, const std::string& channel)
{
    return ":" + _serverName + " 366 " + nick + " " + channel + " :End of /NAMES list";
}


//------------------------------KICK error------------------------------------------------------

std::string Server::ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + _serverName + " 441 " + nick + " " + target_nick + " " + channel + " :They aren't on that channel";
}
std::string Server::MSG_KICK(const std::string& kicker_prefix, const std::string& channel, const std::string& target_nick, const std::string& reason)
{
    return ":" + kicker_prefix + " KICK " + channel + " " + target_nick + " :" + reason;
}




//------------------------------INVIT error------------------------------------------------------
std::string Server::ERR_USERONCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + _serverName + " 443 " + nick + " " + target_nick + " " + channel + " :is already on channel";
}
std::string Server::RPL_INVITING(const std::string& nick, const std::string& target_nick, const std::string& channel)
{
    return ":" + _serverName + " 341 " + nick + " " + target_nick + " " + channel;
}
std::string Server::MSG_INVITE(const std::string& inviter_prefix, const std::string& target_nick, const std::string& channel)
{
    return ":" + inviter_prefix + " INVITE " + target_nick + " :" + channel;
}