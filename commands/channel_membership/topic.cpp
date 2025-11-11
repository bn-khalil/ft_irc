#include "channel.hpp"
#include <sstream>


// static void showChannelTopic(std::map<std::string,Channel*> & channel) {
//     std::map<std::string,Channel*>::iterator it = channel.begin();
//     while (it != channel.end())
//     {

//     }
// }
// static std::vector<std::string> new_splite(std::string &strr, char d)
// {
//     std::string save;
//     std::stringstream ss(strr);
//     std::vector<std::string> resulte;

//     while(getline(ss,  save , d))
//     {
//             resulte.push_back(save);
//     }
//     return resulte;
// }

void  Server::topic(std::vector<std::string> cmds, Client &c) {

    if (cmds.size() == 1)
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), cmds[0], "<channel> [:<topic>]"));
    else if (cmds.size() == 2 ) {
        std::map<std::string,Channel*>::iterator it = this->channel.find(cmds[1]);

        if (it != channel.end()) {
            if (!it->second->getTopic().empty()) {
                sendReply(c, error.RPL_TOPIC(c.get_nickname(), cmds[1], cmds[2]));
            } else
                sendReply(c, error.RPL_NOTOPIC(c.get_nickname(), cmds[1]));
        }
        else
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), cmds[1]));
    } 
    else {
        // std::cout << "->" << c.getlineCmd() << '\n';
        std::vector<std::string> args = cmds;
        std::string topic;

        for(size_t i = 2 ; i < args.size(); i++) {
            topic.append(args[i]);
            topic.append(" ");
        }

        std::map<std::string,Channel*>::iterator it = this->channel.find(args[1]);

        if (it != channel.end()) {
            it->second->setTopic(topic);
            sendReply(c, error.RPL_TOPIC(c.get_nickname(), args[1], topic));
        } else
            sendReply(c, error.ERR_NOSUCHCHANNEL(c.get_nickname(), args[0]));
    }
}

// :*.freenode.net 332 bn #c :
// :bn!~n@freenode-obu.d75.6g0qj4.IP TOPIC #bn :new content