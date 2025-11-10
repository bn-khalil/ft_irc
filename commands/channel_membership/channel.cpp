#include "Channel.hpp"
#include "../../server/Client.hpp"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <utility>

void Channel::Add_to_admin(Client *c)
{
   operators.insert(std::make_pair(c->nickname,c));
}
void Channel::Add_to_user(Client *c)
{
   users.insert(std::make_pair(c->nickname,c));
}
void Channel::Add_to_invite(Client *c)
{
   invite.insert(std::make_pair(c->nickname,c));
}

std::string Channel::Get_key()
{
    return this->key;
}
bool Channel::isClientOperator(Client *c) const
{
    // return (operators.find(c) != operators.end());
}
std::string Channel::Set_key()
{
    return this->key;
}
void Channel::kick_from_channel(Client *c)
{
    if(isClientOperator(c) == true)
        std::cout << "YOU CAN 'T KICK ADMIN" << std::endl;
    // user.erase(c);
}
bool Channel::Check_mode(char mode)
{
    if(mode == 'k')
        return this->isKeySet;
    if(mode == 'i')
        return this->isInviteOnly;
    if(mode == 'l')
        return this->limit;
}

bool Channel::isInvited(Client *c)
{
    return (invite.find(c->nickname) != invite.end());
}
bool Channel::is_full()
{
    return(users.size() >= limit);
}
