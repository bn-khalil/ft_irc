#include "Reply.hpp"

Reply::Reply() : server_name("ircserver42.com")
{
}

std::string Reply::ERR_NEEDMOREPARAMS(std::string nick, std::string command)
{
    std::string msg = ":" + server_name + " 461 " + nick + " " + command + " :Not enough parameters"; 
    return msg;
}

std::string Reply::ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name)
{
    return ":" + server_name + " 403 " + nick + " " + channel_name + " :No such channel";
}

std::string Reply::ERR_NOT_REGESTRED(const std::string& nick)
{
    return ":" + server_name + " 451 " + nick + " :Register first";
}

std::string Reply::ERR_LEAVE_ALL_CHANNEL(const std::string& nick, const std::string& channel_name)
{
    return ":" + server_name + " 403 " + nick + " " + channel_name + " :Left all channels";
}

std::string Reply::ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 482 " + nick + " " + channel + " :You're not channel operator";
}

std::string Reply::ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 442 " + nick + " " + channel + " :You're not on that channel";
}

std::string Reply::ERR_UNKNOWNCOMMAND_N(const std::string &nick, const std::string & command)
{
    return ":*." + server_name + " 421 " + nick + " " + command + " :Unknown command";
}

std::string Reply::ERR_NOTCHANNELOPERATO(const std::string &nick, const std::string & channel) {
    return ":*." + server_name + " 482 " + nick + " " + channel + " : You're not channel operator";
}

std::string Reply::RPL_MODEOPTIONS(const std::string & prefix, const std::string& channel, const std::string& modes) {
    return prefix + " " + channel + " " + modes;
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

std::string Reply::MSG_KICK(const std::string& kicker_prefix, const std::string& channel, const std::string& target_nick, const std::string &reason)
{
    return ":" + kicker_prefix + " KICK " + channel + " " + target_nick + " " + reason;
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

//------------------------------TOPIC error------------------------------------------------------

std::string Reply::RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic)
{
    return ":" + server_name + " 332 " + nick + " " + channel + " :" + topic;
}

std::string Reply::RPL_NOTOPIC(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 331 " + nick + " " + channel + " :No topic is set";
}

/* ************************************************************************** */
///just to compile 
std::string Reply::ERR_NOSUCHNICK(const std::string& nick, const std::string& target)
{
    return ":" + server_name + " 401 " + nick + " " + target + " :No such nick";
}

std::string Reply::RPL_INVITELIST(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 346 " + nick + " :" + channel;
}

std::string Reply::RPL_ENDOFINVITELIST(const std::string& nick)
{
    return ":" + server_name + " 347 " + nick + " :End of Invite List";
}

std::string Reply::ERR_NEEDMODEPARM(const std::string& nick, const std::string& channel, const std::string & mode, const std::string& description)
{
    return ":" + server_name + " 461 " + nick + " " + channel + " " + mode + " :" + description;
}

std::string Reply::ERR_NICKNOTFOUND(const std::string& nick, const std::string& target)
{
    return ":" + server_name + " 401 " + nick + " " + target + " :No such nick/channel";
}

std::string Reply::ERR_KEYALREADYSET(const std::string& nick, const std::string& channel)
{
    return ":" + server_name + " 467 " + nick + " " + channel + " :Channel key already set";
}

std::string Reply::ERR_INVALIDMODEPARM(const std::string& nick, char mode)
{
    std::string m(1, mode);
    return ":" + server_name + " 472 " + nick + " " + m + " :is unknown mode char to me";
}

//------------------------------PRIVMSG error------------------------------------------------------

std::string Reply::ERR_NORECIPIENT(const std::string & nick) 
{
    return ":" + server_name + " 411 " + nick + " :No recipient given (PRIVMSG)";
}

std::string Reply::ERR_NOTEXTSEND(const std::string & nick) 
{
    return ":" + server_name + " 412 " + nick + " :No text to send";
}

std::string Reply::RPL_PRIVMSG(const std::string & prefix, const std::string & nick , const std::string & message) 
{
    return prefix + " PRIVMSG " + nick + " :" + message;
}

std::string Reply::ERR_PASSWDMISMATCH(std::string nick, std::string command)
{
    return ":" + server_name + " 464 " + nick + " " + command;
}
std::string Reply::ERR_ALREADYREGISTERED(std::string nick, std::string command)
{
    return ":" + server_name + " 462 " + nick + " " + command;
}
std::string Reply::ERR_NONICKNAMEGIVEN(std::string nick, std::string command)
{
    return ":" + server_name + " 431 " + nick + " " + command;
}
std::string Reply::ERR_ERRONEUSNICKNAME(std::string nick, std::string command)
{
    return ":" + server_name + " 432 " + nick + " " + command;
}
std::string Reply::ERR_NICKNAMEINUSE(std::string nick, std::string used_nick, std::string command)
{
    return ":" + server_name + " 433 " + nick + " " + used_nick + " " + command;
}
std::string Reply::RPL_CHANNELCREATIONTIME(const std::string& nick, const std::string& time, const std::string & channel)
{
    return ":" + server_name + " 333 " + nick + " " + channel + " " + time;
}
std::string Reply::RPL_CHANNELACTIVEMODES(const std::string& nick, const std::string& modes, const std::string & channel)
{
    return ":" + server_name + " 324 " + nick + " " + channel + " " + modes;
}
