#include "channel.hpp"
#include <sstream>

void  Server::topic(Client &c) {
    std::string command = c.getlineCmd();
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }
    std::vector<std::string> args = new_splite(command, ' ');

    if (args.size() == 1)
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "TOPIC", "<channel> [:<topic>]"));
    else if (args.size() == 2) {
        std::map<std::string,Channel*>::iterator it = this->channel.find(args[1]);
        if (it == channel.end()) {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), args[1]));
            return ;
        }

        if (it->second->getTopic().empty())
            sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), args[1]));
        else
            sendReply(c, error.RPL_TOPIC(c.get_nickname(), args[1], it->second->getTopic()));
        
    } 
    else {

        std::string topic;

        for (size_t i = 2; i < args.size(); ++i) {
            if (i > 2) topic.push_back(' ');
            topic += args[i];
        }

        std::map<std::string,Channel*>::iterator it = this->channel.find(args[1]);

        if (it == channel.end()) {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), args[0]));
            return ;
        }

        if (!it->second->isClientOperator(c) && it->second->getTopicRestriction()) {
            sendReply(c, this->error.ERR_NOTCHANNELOPERATO(c.get_nickname(), it->second->get_channel_name()));
            return ;
        }
    
        it->second->setTopic(topic);
        std::cout << it->second->getTopic() << std::endl;
        sendReply(c, error.RPL_TOPIC(c.get_nickname(), args[1], topic));
        it->second->broadcast(error.RPL_TOPIC(c.get_nickname(), args[1], topic));
        // broadcast shoul
    }
}

// :*.freenode.net 332 bn #c :
// :bn!~n@freenode-obu.d75.6g0qj4.IP TOPIC #bn :new content