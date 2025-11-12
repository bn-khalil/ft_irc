#include "channel.hpp"

void  Server::mode(std::vector<std::string> cmds, Client &c) {
    // <target> [[(+|-)]<modes> [<mode-parameters>]]
    if (cmds.size() == 1) 
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "MODE", "<target> [[(+|-)]<modes> [<mode-parameters>]]"));
    else if (cmds.size() == 2) {
        std::map<std::string,Channel*>::iterator it = this->channel.find(cmds[1]);
        if (it == channel.end()) {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), cmds[1]));
            return ;
        }
    }
    else if (cmds.size() == 2) {
        
    }
}
