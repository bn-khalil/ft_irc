#include "channel.hpp"
#include "../../server/Server.hpp"

void Server::Quit(Client &c)
{
    std::vector<std::string> channel_to_leave;
    std::map<std::string, Channel>::iterator it = this->channel.begin();
    while (it != this->channel.end())
    {
        Channel &ch = it->second;
        if (ch.isUserInChannel(c) == true)
            channel_to_leave.push_back(ch.get_channel_name());
        it++;
    }
    for (size_t i = 0; i < channel_to_leave.size(); i++)
    {
        std::map<std::string, Channel>::iterator it1 = channel.find(channel_to_leave[i]);
        if (it1 == channel.end())
            continue;
        Channel &chan = it1->second;
        chan.broadcast(error.MSG_PART(c.get_Prefix(), chan.get_channel_name(), "QUIT :EOF from client"));
        chan.removeClientFromOneChannels(c);
        if (chan.isEmpty() == true)
            channel.erase(it1);
    }

}