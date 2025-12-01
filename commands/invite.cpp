#include "channel.hpp"

void Server::invit(Client &c)
{
    std::string command = c.getlineCmd();

    if (!command.empty() && command[command.size() -1 ] == '\n')
        command.erase(command.size() -1);

    std::vector<std::string> cmds = new_splite(command, ' ');
    if (c.Get_isAuthenticated() == false)
        return sendReply(c, error.ERR_NOT_REGESTRED());

    if (cmds.size() > 2)
    {
        std::string one_channel = cmds[2];
        std::string name_c_invited = cmds[1];

        std::map<std::string, Channel>::iterator it = channel.find(toLower(one_channel));
        if (one_channel.empty() || (one_channel[0] != '&' && one_channel[0] != '#') || one_channel.length() > 200 || Channel::whitespace_found(one_channel) == true  || it == channel.end())
        {
         return sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), one_channel));
        }
         Client *client_invited = find_client_by_nickname(name_c_invited);

        if (!client_invited)
            return sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), name_c_invited));
        Channel &real_one = it->second;
        if(real_one.isUserInChannel(c) == false)
            return sendReply(c,error.ERR_NOTONCHANNEL(c.get_nickname(),one_channel));
        if (real_one.isUserInChannel(*client_invited) == true)
            return sendReply(c, error.ERR_USERONCHANNEL(c.get_nickname(), name_c_invited, one_channel));
        if (real_one.isClientOperator(c) == false)
            return sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), one_channel));

        real_one.Add_to_invite(*client_invited);
        sendReply(c, error.RPL_INVITING(c.get_nickname(), name_c_invited, one_channel));
        sendReply(*client_invited, error.MSG_INVITE(c.get_Prefix(), name_c_invited, one_channel));
    }
    else if (cmds.size() == 1)
    {
        std::vector<std::string> all_invited_channel = isUserInvited_to_channel(c);
        for (size_t i = 0; i < all_invited_channel.size(); i++)
            sendReply(c, error.RPL_INVITELIST(c.get_nickname(), all_invited_channel[i]));
        
        sendReply(c, error.RPL_ENDOFINVITELIST(c.get_nickname()));
    }
    else
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "INVITE"));
}