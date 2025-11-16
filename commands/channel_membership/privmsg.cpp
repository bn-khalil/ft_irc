#include "channel.hpp"

void Server::privmsg(Client &c) {
    std::string command = c.getlineCmd();
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }

    if (c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
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
        std::string rcvNick = args[1];
        bool isChannel = false;
        if (!rcvNick.empty() && (rcvNick[0] == '#' || rcvNick[0] == '&'))
            isChannel = true;
            
        if (!isChannel) {
            Client *rcvClient = find_client_by_nickname(rcvNick);
            if (!rcvClient) {
                sendReply(c, error.ERR_NOSUCHNICK(c.get_nickname(), rcvNick));
                return;
            }
            sendReply(*rcvClient, error.RPL_PRIVMSG(c.get_Prefix(),
                      rcvClient->get_nickname(), message));
        } else {
            std::map<std::string, Channel>::iterator it = this->channel.find(rcvNick);
            
            if (it == this->channel.end()) {
                sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), rcvNick));
                return;
            }
            if (rcvNick[0] == '&')
                return;
                
        it->second.broadcastExpectSender(error.RPL_PRIVMSG(c.get_Prefix(), it->second.get_channel_name(), message), c);
        }
    }
}