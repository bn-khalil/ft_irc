#ifndef REPLY_HPP
# define REPLY_HPP

# include <string>

class Reply
{
	private:
		std::string server_name;
	public:

		Reply();
		

	std::string ERR_PASSWDMISMATCH(std::string nick, std::string command);
	std::string ERR_ALREADYREGISTERED(std::string nick, std::string command);
	std::string ERR_NONICKNAMEGIVEN(std::string nick, std::string command);
	std::string RPL_CREATED(const std::string& nick, const std::string& creation_date);
	
	std::string RPL_WELCOME(const std::string& nick, const std::string& user_prefix);
	std::string RPL_YOURHOST(const std::string& nick);
	std::string ERR_NICKNAMEINUSE(std::string nick, std::string used_nick, std::string command);
	std::string ERR_ERRONEUSNICKNAME(std::string nick, std::string used_nick, std::string command);
	std::string RPL_MYINFO(const std::string& nick);
		
		// --- General Errors ---
    	std::string ERR_NEEDMOREPARAMS(std::string nick, std::string command);
    	std::string ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name);
    	std::string ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel);
    	std::string ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel);
    	std::string ERR_NOT_REGESTRED();
		std::string ERR_UNKNOWNCOMMAND_N(const std::string &nick, const std::string & command);
		std::string ERR_INPUTTOOLONG(const std::string & nick);

		// --- General msgs ---
    	std::string RPL_BROADCAST(const std::string& nick, const std::string & command, const std::string & channel, const std::string & message);
		
    	// --- JOIN Part ---
    	std::string ERR_BADCHANNELKEY(const std::string& nick, const std::string& channel);
    	std::string ERR_INVITEONLYCHAN(const std::string& nick, const std::string& channel);
    	std::string ERR_CHANNELISFULL(const std::string& nick, const std::string& channel);
    	std::string MSG_JOIN(const std::string& user_prefix, const std::string& channel_name);
    	std::string RPL_NAMREPLY(const std::string& nick, const std::string& channel, const std::string& names_list);
    	std::string RPL_ENDOFNAMES(const std::string& nick, const std::string& channel);
    	std::string RPL_ENDOFINVIT(const std::string& nick ,const std::string& channel);
    	std::string ERR_LEAVE_ALL_CHANNEL(const std::string& nick ,const std::string& channel);

        // --- KICK Part ---
    	std::string ERR_USERNOTINCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel);
    	std::string MSG_KICK(const std::string& kicker_prefix, const std::string& channel, const std::string& target_nick,const std::string& reason);
		std::string	MSG_PART(const std::string& user_prefix, const std::string& channel, const std::string& reason);
        // --- INVITE Part ---
    	std::string ERR_USERONCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel);
    	std::string RPL_INVITING(const std::string& nick, const std::string& target_nick, const std::string& channel);
    	std::string MSG_INVITE(const std::string& inviter_prefix, const std::string& target_nick, const std::string& channel);
		// Client client;

		// --- TOPIC part ---
    	std::string RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic);
    	std::string RPL_TOPICREATED(const std::string& nick, const std::string& prefix, const std::string& topic);
    	std::string RPL_NOTOPIC(const std::string& nick, const std::string& channel);
		std::string ERR_NOSUCHNICK(const std::string& nick, const std::string& target_nick);
		std::string RPL_INVITELIST(const std::string& nick, const std::string& channel);
		std::string RPL_ENDOFINVITELIST(const std::string& nick);
		std::string RPL_TOPICWHOTIME (const std::string& nick, const std::string& time, const std::string & channel, const std::string & topicChanger);

		// --- MODE part ---
		std::string ERR_NEEDMODEPARM(const std::string& nick, const std::string & mode, const std::string& more);
		std::string ERR_UNKNOWNMODE(const std::string& nick, const char mode);
		std::string ERR_KEYALREADYSET(const std::string& nick, const std::string& channelname);
		std::string ERR_NICKNOTFOUND(const std::string& nick, const std::string& otherNick);

		std::string RPL_MODEOPTIONS(const std::string & prefix, const std::string& channel, const std::string& modes);
    	std::string  RPL_CREATIONTIME (const std::string& nick, const std::string& time, const std::string & channel);
    	std::string  RPL_CHANNELMODEIS(const std::string& nick, const std::string& modes, const std::string & channel);

		// --- PRIVMSG ----
		std::string ERR_NORECIPIENT(const std::string & nick);
		std::string ERR_NOTEXTSEND(const std::string & nick);
		std::string ERR_CANNOTSENDTOCHAN(const std::string & nick , const std::string & channel);

		std::string RPL_AWAY(const std::string & prefix, const std::string & nick , const std::string & message);


};


#endif
