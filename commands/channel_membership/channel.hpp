#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <map>
#include <vector>
#include "../../server/Client.hpp"
#include <vector>
#include "../../server/Client.hpp"
#include "../../server/Server.hpp"
#include <sys/socket.h>
#include "../../server/Client.hpp"
#include <sys/socket.h>
#include <map>

class Channel 
{
    private:
        std::string Channel_name;
        std::map<std::string,Client*> operators_;
        std::map<std::string,Client*> users;
        std::map<std::string,Client*> invite;
        std::string key;
        bool isKeySet;
        bool isInviteOnly;
        bool  limit;
        unsigned int num_limite;
        
    public :
        std::map<std::string,Client*> get_operators_(){return operators_;};
        std::map<std::string,Client*> get_users(){return users;};
         std::string getNamesList();
        unsigned int get_num_limite() {return num_limite;};
        std::string get_channel_name(){return Channel_name;};
        Channel(std::string name);
        void Add_to_admin(Client &c);
        void Add_to_user(Client &c);
        void Add_to_invite(Client &c);
        void kick_from_channel(Client &c);
        bool isClientOperator(Client &c) const;
        void join(std::vector<std::string> cmds, Client &c);
	    std::vector<std::string> splite_coma(std::string &strr, char d);
        std::string  Get_key();
        std::string  Set_key();
        bool Check_mode(char mode);
        bool isInvited(Client &c);
        bool isUserInChannel(Client &c);
        bool is_full();
        void rm_user_from_channel(Client &c);
        bool  isEmpty();
        bool    isClientUSER(Client &c) const;
        std::string to_lower(std::string str);

};


#endif