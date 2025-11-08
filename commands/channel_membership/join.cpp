#include <cstdio>
#include <sstream>
#include <vector>
#include "../../server/Client.hpp"
#include "../../server/Server.hpp"
#include "channel.hpp"
#include <sys/socket.h>

//splite by comma
//check
void Channel::sendReply(Client *c, std::string msg)
{
    
    std::string full_msg = msg + "\r\n";
    // send(c->sockfd , full_msg.c_str() , full_msg.length(), 0);
    //check the return value of send if 0 or if -1 

    //-1 mean the connect  stop not cleanly 
    //0 the connecttions stop cleanly

}
std::vector<std::string> splite_coma(std::string &strr, char d)
{
    std::string save;
    std::stringstream ss(strr);
    std::vector<std::string> resulte;

    while(getline(ss,  save , d))
    {
            resulte.push_back(save);
    }
    return resulte;
}
void Server::join(std::vector<std::string> cmds, Client *c)
{
    if(cmds.size() < 2)
    {
        std::cout << "send specific number to clien" << std::endl;
        // sendReply(c, "CHECK_LATER_V1");
        return ;
    }
    int flage = 10;
    if(cmds[1].find(","))
        flage = 1;
    if(cmds[2].find(","))
        flage = 2;

    if(flage == 1)
        cmds = splite_coma(cmds[1], ',');
    if(flage == 2)
        cmds = splite_coma(cmds[2], ',');

    if(cmds[0] != "JOIN")
    {
        return ;
    }
    std::string Channel_name = cmds[1];
    if(Channel_name.length() < 2 || Channel_name[0] != '&' || Channel_name[0] != '#' || Channel_name.length() > 200)
    {
        // sendReply(c, "CHECK_LATER_V2");
        return ;
    }
    Channel *join = NULL;;
    std::map<std::string, Channel *>::iterator it = channel.find(Channel_name);
    // first time no channel in the map
    if(it == channel.end())
    {
        join = new Channel(Channel_name);
        channel[Channel_name] = join;
        join->Add_to_admin(c);
        join->Add_to_user(c);
    }
    // secend time  we have alreddy the channel
    else
    {
        join = it->second;
        join->Add_to_user(c);
    }
    }