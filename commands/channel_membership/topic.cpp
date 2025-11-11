#include "channel.hpp"

static void setChannelTopic(std::map<std::string,Channel*> & channel, std::vector<std::string> cmds) {
    std::map<std::string,Channel*>::iterator it = channel.begin();
    while (it != channel.end())
    {
        if (it->first == cmds[1]) {
            it->second->setTopic(cmds[2]);
            std::cout << "topic changed!" << std::endl;
        }
    }
    
}

static void showChannelTopic(std::map<std::string,Channel*> & channel) {
    std::map<std::string,Channel*>::iterator it = channel.begin();
    while (it != channel.end())
    {

    }
}

void  Server::topic(std::vector<std::string> cmds, Client &c) {
    if (cmds.size() == 1)
        sendReply(, error.ERR_NOSUCHCHANNEL());
    else
        setChannelTopic(this->channel, cmds);
}