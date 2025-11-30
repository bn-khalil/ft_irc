#include "channel.hpp"

void Server::privmsg(Client &c) {
    std::string command = c.getlineCmd();
    if (!command.empty() && command[command.size() -1 ] == '\n') {
        command.erase(command.size() -1);
    }

    if (c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED());
        return;
    }
    
    std::vector<std::string> args = new_splite(command, ' ');

    int dotsIndex = command.find(":");
    std::string message;
    if (dotsIndex < 0)
        message = args[args.size() - 1];
    else
        message = command.substr(dotsIndex + 1);

    if (args.size() <= 1)
        sendReply(c, error.ERR_NORECIPIENT(c.get_nickname()));
    else if (args.size() == 2)
        sendReply(c, error.ERR_NOTEXTSEND(c.get_nickname()));
    else {
        std::vector<std::string> multi_users_or_channels = Channel::split_comma(args[1], ',');
        for (size_t i = 0; i < multi_users_or_channels.size(); i++) {
            std::string rcvNick = multi_users_or_channels[i];
            size_t j = 0;
            bool duplicate = false;
            while (j < i) {
                if (multi_users_or_channels[j] == multi_users_or_channels[i])
                    duplicate = true;
                j++;
            }
            if (duplicate)
                continue;
            bool isChannel = false;
            if (!rcvNick.empty() && (rcvNick[0] == '#' || rcvNick[0] == '&'))
                isChannel = true;
                
            if (!isChannel) {
                Client *rcvClient = find_client_by_nickname(rcvNick);
                if (!rcvClient || rcvClient->Get_isAuthenticated() == false) 
                {
                    sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), rcvNick));
                    return;
                }
                if (message.empty()) {
                    sendReply(c, error.ERR_NOTEXTSEND(c.get_nickname()));
                    return ;
                }
                sendReply(*rcvClient, error.RPL_AWAY(c.get_Prefix(),
                        rcvClient->get_nickname(), message));
            } else {
                std::map<std::string, Channel>::iterator it = this->channel.find(toLower(rcvNick));
                if (it == this->channel.end()) {
                    sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), rcvNick));
                    return;
                }
                if (!it->second.isUserInChannel(c)) {
                    sendReply(c, error.ERR_CANNOTSENDTOCHAN(c.get_nickname(), rcvNick));
                    return;
                }
                if (rcvNick[0] == '&')
                    return;
                if (message.empty()) {
                    sendReply(c, error.ERR_NOTEXTSEND(c.get_nickname()));
                    return ;
                }
                it->second.broadcastExpectSender(error.RPL_AWAY(c.get_Prefix(), it->second.get_channel_name(), message), c);
            }
        }
    }
}