#ifndef REPLY_HPP
# define REPLY_HPP

// # include <iostream>
# include <string>

class Reply
{
	private:
		std::string server_name;
	public:

		Reply();
		// --- General Errors ---
    	std::string ERR_NEEDMOREPARAMS(std::string nick, std::string command);
    	std::string ERR_NOSUCHCHANNEL(const std::string& nick, const std::string& channel_name);
    	std::string ERR_CHANOPRIVSNEEDED(const std::string& nick, const std::string& channel);
    	std::string ERR_NOTONCHANNEL(const std::string& nick, const std::string& channel);
    	std::string ERR_NOT_REGESTRED(const std::string& nick);
		std::string ERR_UNKNOWNCOMMAND_N(const std::string &nick, const std::string & command);
		std::string ERR_NOTCHANNELOPERATO(const std::string &nick, const std::string & channel);

        //:tngnet.nl.quakenet.org 482 sf #bn :You're not channel operator

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
    	std::string MSG_KICK(const std::string& kicker_prefix, const std::string& channel, const std::string& target_nick, const std::string& reason);
		std::string	MSG_PART(const std::string& user_prefix, const std::string& channel, const std::string& reason);
        // --- INVITE Part ---
    	std::string ERR_USERONCHANNEL(const std::string& nick, const std::string& target_nick, const std::string& channel);
    	std::string RPL_INVITING(const std::string& nick, const std::string& target_nick, const std::string& channel);
    	std::string MSG_INVITE(const std::string& inviter_prefix, const std::string& target_nick, const std::string& channel);
		// Client client;

		// --- TOPIC part ---
    	std::string RPL_TOPIC(const std::string& nick, const std::string& channel, const std::string& topic);
    	std::string RPL_NOTOPIC(const std::string& nick, const std::string& channel);
		// std::string RPL_TOPICWHOTIME(const std::string& nick, const std::string& channel, const std::string& set_by, const std::string& timestamp)
		std::string ERR_NOSUCHNICK(const std::string& nick, const std::string& target_nick);
		std::string RPL_INVITELIST(const std::string& nick, const std::string& channel);
		std::string RPL_ENDOFINVITELIST(const std::string& nick);

		// --- MODE part ---
		std::string ERR_NEEDMODEPARM(const std::string& nick, const std::string& channelname, const char mode, const std::string& more);
		std::string ERR_INVALIDMODEPARM(const std::string& nick, const char mode);
		std::string WITHTOPIC(const std::string& nick, const std::string& channel, std::string more);
		std::string ERR_KEYALREADYSET(const std::string& nick, const std::string& channelname);
		std::string ERR_NICKNOTFOUND(const std::string& nick, const std::string& otherNick);

		std::string RPL_MODEOPTIONS(const std::string & prefix, const std::string& channel, const std::string& modes);
};


#endif