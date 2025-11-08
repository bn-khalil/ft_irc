#include "Channel.hpp"
#include "../../server/Client.hpp"

void Channel::Add_to_admin(Client *client)
{
   operators.insert(client);
}
void Channel::Add_to_user(Client *client)
{
   user.insert(client);
}

std::string Channel::Get_key()
{
    return this->key;
}
bool Channel::isClientOperator(Client *c) const
{
    return (operators.find(c) != operators.end());
}
std::string Channel::Set_key()
{
    return this->key;
}
void Channel::kick_from_channel(Client *c)
{
    if(isClientOperator(c) == true)
        std::cout << "YOU CAN 'T KICK ADMIN" << std::endl;
    user.erase(c);
}
