#include "channel.hpp"

static bool isChannelModes( char m ) {
    if (m == 'i' || m == 'o' || m == 'k' || m == 't' || m == 'l')
        return true;
    return false;
}

bool Server::isChannelExist(std::map<std::string,Channel*>::iterator & it_channel, 
std::vector<std::string> cmds, Client &c){
    std::map<std::string,Channel*>::iterator it = this->channel.find(cmds[1]);
    if (it == channel.end()) {
        sendReply(c, this->error.ERR_NOSUCHCHANNEL(c.get_nickname(), cmds[1]));
        return false;
    }
    it_channel = it;
    return true;
}

std::vector<modes_t> Server::parseModes(std::vector<std::string> cmds, 
Client &c, 
const std::map<std::string,Channel*>::iterator & it_channel) {

    std::vector<modes_t> modesWithInfo;

    if (cmds.size() <= 2)
        return modesWithInfo;

    size_t indexParam = 2;
    bool sing = true;

    for (;indexParam < cmds.size();) {

        std::string modes = cmds[indexParam];

        for (size_t i = 0; i < modes.size(); i++)
        {
            if (modes[i] == '+')
                sing = true;
            else if (modes[i] == '-')
                sing = false;
            else if (isChannelModes(modes[i])) {
                modes_t currentMode;
                currentMode.sing = sing;
                currentMode.mode = modes[i];
                if (modes[i] == 'k' || (modes[i] == 'l' && sing == true) || modes[i] == 'o') {
                    std::cout << modes[i] << std::endl;
                    indexParam++;
                    if (indexParam < cmds.size()) {
                        currentMode.param = cmds[indexParam];
                    } else {
                        if (modes[i] == 'k')
                            sendReply(c, error.ERR_NEEDMODEPARM(c.get_nickname(), 
                            it_channel->second->get_channel_name(),modes[i], " Syntax : <key>."));
                        else if (modes[i] == 'l')
                            sendReply(c, error.ERR_NEEDMODEPARM(c.get_nickname(), 
                            it_channel->second->get_channel_name(),modes[i], " Syntax : <limit>."));
                        else if (modes[i] == 'o')
                            sendReply(c, error.ERR_NEEDMODEPARM(c.get_nickname(), 
                            it_channel->second->get_channel_name(),modes[i], " Syntax : <nick>."));
                        continue ;
                    }
                }
                modesWithInfo.push_back(currentMode);
            } else 
                sendReply(c, error.ERR_INVALIDMODEPARM(c.get_nickname(), modes[i]));
        }
        indexParam++;
    }
    return modesWithInfo;
}

bool validateLimitParams(std::string limit) {
    for(size_t i = 0; i < limit.size(); i++) {
        if (!std::isdigit(limit[i]) && !std::isspace(limit[i]) && limit[i] != '+')
            return false;
    }
    return true;
}

std::map<std::string,Client*>::iterator Channel::findClientByNickName( const std::string & nickname ) {
    return this->users.find(nickname);
}


void Channel::modeExecuter(modes_t & mode, Client &c) {
    std:: cout << " ==== " << this->isClientOperator(c) << std::endl;
    if (!this->isClientOperator(c)) {
        std::cout << "client not operator" << std::endl;
        return ;
    }

    if (mode.mode == 'i') {        if (mode.sing && !this->isInviteOnly) {
            this->isInviteOnly = true;
            //send to user;
            // std::cout << this->Channel_name << "  -> +i" << std::endl;
        }
        else if (!mode.sing && this->isInviteOnly){
            this->isInviteOnly = false;
            //send to user;
            // std::cout << this->Channel_name << "  -> -i" << std::endl;
        }
    }
    else if (mode.mode == 'l') {
        if (!mode.sing) {
            this->num_limite = 0;
            this->isLimited = false;
        }
        else {
            if (!validateLimitParams(mode.param)) {
                // send message to client;
                std::cout << "param invalid" << std::endl;
                return;
            }

            size_t count = 0;
            for (size_t i = 0; i < mode.param.size(); i++) {
                if (mode.param[i] == '+')
                    count++;
                else if (std::isdigit(mode.param[i]))
                    break ;
            }
            if (count > 1 || count == mode.param.size())
                return ;

            unsigned int limit = static_cast<unsigned int>(std::atol(mode.param.c_str()));

            if (limit < 0 || limit == this->num_limite)
                return ;

            this->num_limite = (limit);
            this->isLimited = true;
            std::cout << "channel limit sat " << this->num_limite << std::endl;
        }
        //:*.freenode.net 696 bn #bn l -4 :Invalid limit mode parameter. Syntax: <limit>.
    }
    else if (mode.mode == 'k') {
        if (this->isKeySet && mode.sing) {
            std::cout << "key already set " << std::endl;
        } else {
            if (!mode.sing && this->key == mode.param) {
                this->isKeySet = false;
                this->key = "";
            } 
            else if (mode.sing) {
                this->isKeySet = true;
                this->key = mode.param;
                std::cout << "key added " << std::endl;
            }
            else
                std::cout << "key already set " << std::endl;
        }
    }
    else if (mode.mode == 'o') {
        std::string nickName = mode.param;
        std::map<std::string,Client*>::iterator client = this->findClientByNickName(nickName);
        if (client == this->users.end()) {
            // send
            //:atw.hu.quakenet.org 401 bn sd :No such nick
            std::cout << "NO suck nick" << std::endl;
            return ;
        }
        if (!mode.sing && isClientOperator( *client->second )) {
            this->popClientFromOperatorList( nickName );
            // send 
            std::cout << nickName << " not an operator in " << this->Channel_name << std::endl;
        }
        else if (mode.sing && !isClientOperator( *client->second )) {

            this->Add_to_admin( *client->second );
            // send 
            std::cout << nickName << " become an operator in " << this->Channel_name << std::endl;
        }
    }
}

void Channel::popClientFromOperatorList( const std::string & nickname ) {
    this->operators_.erase( nickname );
}

void  Server::mode(std::vector<std::string> cmds, Client &c) {

    // std::string command = c.getlineCmd();
    std::vector<modes_t> modes;
    std::map<std::string,Channel*>::iterator it_channel = this->channel.end();

    // if (!command.empty() && command.back() == '\n') {
    //     command.pop_back();
    // }
    // std::vector<std::string> cmds = new_splite(command, ' ');

    if (cmds.size() == 1) 
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "MODE", "<target> [[(+|-)]<modes> [<mode-parameters>]]"));
    else if (cmds.size() == 2) {
        if (!isChannelExist(it_channel, cmds, c))
            return ;
    }
    else {
        if (!isChannelExist(it_channel, cmds, c))
            return ;

        modes = parseModes(cmds, c, it_channel);
        for (size_t i = 0; i < modes.size(); i++) {
            std:: cout << modes[i].sing << modes[i].mode << " --> " << modes[i].param << std::endl;
            it_channel->second->modeExecuter(modes[i], c);
        }
    }
    modes.clear();
}
