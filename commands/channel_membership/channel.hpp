#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <cstddef>
#include <map>
#include <vector>
#include "../../server/Server.hpp"
#include <sys/socket.h>
#include "../../server/Client.hpp"
#include <cstdlib>

class Channel 
{
    private:
        std::string Channel_name;
        std::map<std::string,Client*> operators_;
        std::map<std::string,Client*> users;
        std::map<std::string,Client*> invite;
        std::string key;
        std::string channelTopic;
        bool isKeySet;
        bool isInviteOnly;
        bool limit;
        bool isLimited;
        bool topicRestriction;
        unsigned int num_limite;
        time_t time_creat_channel;
        
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
	    static std::vector<std::string> splite_coma(std::string &strr, char d);
        std::string  Get_key();
        std::string  Set_key();
        bool Check_mode(char mode);
        bool isInvited(Client &c);
        bool is_full();
        void rm_user_from_channel(Client &c);
        bool  isEmpty();
        bool    isClientUSER(Client &c) const;
        static std::string to_lower(std::string str);
        void setTopic( std::string topic );
        std::string getTopic();
        void broadcast(const std::string &msg);
        void broadcastExpectSender(const std::string &msg, Client & sender);
        bool modeExecuter(modes_t & mode, Client &c, Server & server);
        std::map<std::string,Client*>::iterator findClientByNickName( const std::string & nickname );
        void popClientFromOperatorList( const std::string & nickname );
        bool getTopicRestriction();
        bool isUserInChannel(Client &c);
        void removeClientFromOneChannels(Client &c);
        size_t get_number_of_users()
        {
            return(users.size());
        }
};


#endif