#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <system_error>
#include <vector>
#include <set>
#include "Client.hpp"

class Channel 
{
    private:

        std::set<Client*> operators;
        std::set<Client*> user;
        std::string key;

    public :
        void Add_to_admin(Client *c);
        void Add_to_user(Client *c);
        void kick_from_channel(Client *c);
        bool isClientOperator(Client *c) const;
        
        std::string  Get_key();
        std::string  Set_key();

};


#endif