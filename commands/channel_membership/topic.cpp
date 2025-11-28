#include "channel.hpp"
#include <string>

void  Server::topic(Client &c) {

    if(c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED());
        return ;
    }
    
    std::string command = c.getlineCmd();
    if (!command.empty() && command[command.size() -1 ] == '\n') {
        command.erase(command.size() -1);
    }
    std::vector<std::string> args = new_splite(command, ' ');
    args[1] = toLower(args[1]);

    if (args.size() == 1)
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "TOPIC"));
    else if (args.size() == 2) {
        std::map<std::string,Channel>::iterator it = this->channel.find(args[1]);
        if (it == channel.end()) {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), args[1]));
            return ;
        }

        if (it->second.getTopic().empty())
            sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), args[1]));
        else {
            sendReply(c, error.RPL_TOPIC(c.get_nickname(), args[1], it->second.getTopic()));
            sendReply(c, error. RPL_CREATIONTIME (
                c.get_nickname(), it->second.fromTime(it->second.getTimeTopic()), it->second.get_channel_name()));
        }
    } 
    else {

        int dotsIndex = command.find(":");
        std::string topic;
        if (dotsIndex < 0)
            topic = args[args.size() - 1];
        else
            topic = command.substr(dotsIndex + 1);

        std::map<std::string,Channel>::iterator it = this->channel.find(args[1]);

        if (it == channel.end()) {
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), args[0]));
            return ;
        }
        
        if (!it->second.isUserInChannel(c))
        {
            sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(), it->first));
            return;
        }

        if (!it->second.isClientOperator(c) && it->second.getTopicRestriction()) {
            sendReply(c, this->error.ERR_CHANOPRIVSNEEDED(c.get_nickname(), it->second.get_channel_name()));
            return ;
        }
        it->second.setTopic(topic);
        it->second.setTimeTopic(std::time(0));
        it->second.broadcast(error.RPL_TOPICREATED(it->second.get_channel_name(), c.get_Prefix(), topic));
    }
}