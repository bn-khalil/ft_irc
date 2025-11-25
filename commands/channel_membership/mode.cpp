#include "channel.hpp"
#include <cstddef>
#include <map>
#include <vector>

static bool isChannelModes(char m) {
    if (m == 'i' || m == 'o' || m == 'k' || m == 't' || m == 'l')
        return true;
    return false;
}

bool Server::isChannelExist(std::map<std::string, Channel>::iterator &it_channel, 
                            std::vector<std::string> cmds, Client &c) {
    std::map<std::string, Channel>::iterator it = this->channel.find(cmds[1]);
    if (it == channel.end()) {
        sendReply(c, this->error.ERR_NOSUCHCHANNEL(c.get_nickname(), cmds[1]));
        return false;
    }
    it_channel = it;
    return true;
}

std::vector<modes_t> Server::parseModes(std::vector<std::string> cmds, Client &c) {
    std::vector<modes_t> modesWithInfo;

    if (cmds.size() <= 2)
        return modesWithInfo;

    size_t indexParam = 2;
    bool sing = true;

    for (; indexParam < cmds.size();) {
        std::string modes = cmds[indexParam];
        for (size_t i = 0; i < modes.size(); i++) {
            if (modes[i] == '+')
                sing = true;
            else if (modes[i] == '-')
                sing = false;
            else if (isChannelModes(modes[i])) {
                modes_t currentMode;
                currentMode.sing = sing;
                currentMode.mode = modes[i];
                currentMode.display = false;
                if (modes[i] == 'k' || (modes[i] == 'l' && sing == true) || modes[i] == 'o') {
                    indexParam++;
                    if (indexParam < cmds.size()) {
                        currentMode.param = cmds[indexParam];
                        if (cmds[indexParam][0] == ':') {
                            currentMode.param = cmds[indexParam].substr(1);
                            while (indexParam < cmds.size())
                                indexParam++;
                        }
                    } else {
                        std::string modeWithFlag(1, modes[i]);
                        if (modes[i] == 'k')
                            sendReply(c, error.ERR_NEEDMODEPARM(c.get_nickname(), 
                                    modeWithFlag, "Not enough parameters"));
                        else if (modes[i] == 'l')
                            sendReply(c, error.ERR_NEEDMODEPARM(c.get_nickname(), 
                                    modeWithFlag, "Not enough parameters"));
                        else if (modes[i] == 'o')
                            continue ;
                        continue;
                    }
                }
                modesWithInfo.push_back(currentMode);
            } else
                sendReply(c, error.ERR_UNKNOWNMODE(c.get_nickname(), modes[i]));
        }
        indexParam++;
    }
    return modesWithInfo;
}

bool validateLimitParams(std::string limit) {
    size_t i = 0;
    while (std::isspace(limit[i]))
        i++;
    if (i == limit.size())
        return false;
    if (limit[i] == '+')
        i++;
    if (!std::isdigit(limit[i]) && !std::isspace(limit[i]))
        return false;
    return true;
}

std::map<std::string, Client*>::iterator Channel::findClientByNickName(const std::string &nickname) {
    return this->users.find(nickname);
}

bool Channel::modeExecuter(modes_t &mode, Client &c, Server &server) {

    if (mode.mode == 'i') { 
        if (mode.sing && !this->isInviteOnly) {
            this->isInviteOnly = true;
        }
        else if (!mode.sing && this->isInviteOnly) {
            this->isInviteOnly = false;
        }
        else
            return false;
    }
    else if (mode.mode == 't') {
        if (mode.sing && !this->topicRestriction)
            this->topicRestriction = true;
        else if (!mode.sing && this->topicRestriction)
            this->topicRestriction = false;
        else 
            return false;
    }
    else if (mode.mode == 'l') {
        if (!mode.sing) {
            this->num_limite = 0;
            this->isLimited = false;
        }
        else {
            if (!validateLimitParams(mode.param))
                return false;

            unsigned int limit = static_cast<unsigned int>(std::atol(mode.param.c_str()));

            if (limit == this->num_limite)
                return false;
            mode.param = fromTime(limit);
            this->num_limite = (limit);
            this->isLimited = true;
        }
    }
    else if (mode.mode == 'k') {
        if (this->isKeySet && mode.sing) {
            server.sendReply(c, server.error.ERR_KEYALREADYSET(c.get_nickname(), this->Channel_name));
            return false;
        }
        else {
            if (!mode.sing && this->key == mode.param) {
                this->isKeySet = false;
                this->key = "";
            } 
            else if (mode.sing) {
                this->isKeySet = true;
                this->key = mode.param;
            }
            else {
                server.sendReply(c, server.error.ERR_KEYALREADYSET(c.get_nickname(), this->Channel_name));
                return false;
            }
        }
    }
    else if (mode.mode == 'o') {
        std::string nickName = mode.param;
        Client *client = server.find_client_by_nickname(nickName);
        if (!client) {
            server.sendReply(c, server.error.ERR_NOSUCHNICK(c.get_nickname(), nickName));
            return false;
        }
        if (!isUserInChannel(*client)) {
            server.sendReply(c, server.error.ERR_USERNOTINCHANNEL(c.get_nickname(), nickName, this->get_channel_name()));
            return false;
        }
        if (!mode.sing && isClientOperator(*client))
            this->popClientFromOperatorList(nickName);
        else if (mode.sing && !isClientOperator(*client))
            this->Add_to_admin(*client);
        else
            return false;
    }
    return true;
}

void Channel::popClientFromOperatorList(const std::string &nickname) {
    this->operators_.erase(nickname);
}

std::string Channel::removeDuplicate(const std::string &s) {
    std::set<char> eles;
    std::string result;

    for (size_t i = 0; i < s.size(); ++i) {
        if (eles.find(s[i]) == eles.end()) {
            eles.insert(s[i]);
            result += s[i];
        }
    }
    return result;
}

static void prepareModesMessage(std::map<char, modes_t>::iterator hold, 
std::string & sortModesPlus,
 std::string & sortModesMinus, 
 std::vector<std::string> & seccessModes) {
    if (hold->second.param.empty()) {
        if (hold->second.sing)
            sortModesPlus = hold->second.mode + sortModesPlus;
        else
            sortModesMinus = hold->second.mode + sortModesMinus;
    }
    else {
        if (hold->second.sing)
            sortModesPlus += hold->second.mode;
        else
            sortModesMinus += hold->second.mode;
        seccessModes.push_back(hold->second.param);
    }
}

void Server::mode(Client &c) {
    std::vector<modes_t> modes;
    std::vector<modes_t> filterdModes;
    std::vector<modes_t> filterdModesCopy;
    std::map<char, modes_t> filerM;
    std::map<std::string, Channel>::iterator it_channel = this->channel.end();
    std::vector<std::string> seccessModes;
    std::string sortModes;
    std::string sortModesPlus;
    std::string sortModesMinus;
    old_channel_modes_t channelCurrentModes; 

    if (c.Get_isAuthenticated() == false)
    {
        sendReply(c, error.ERR_NOT_REGESTRED(c.get_nickname()));
        return;
    }

    std::string command = c.getlineCmd();
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }
    std::vector<std::string> cmds = new_splite(command, ' ');

    if (cmds.size() == 1) 
        sendReply(c, error.ERR_NEEDMOREPARAMS(c.get_nickname(), "MODE"));
    else if (cmds.size() == 2) {
        if (!isChannelExist(it_channel, cmds, c))
            return;
        if (!it_channel->second.isUserInChannel(c)) {
            sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(),it_channel->second.get_channel_name()));
            return;
        }
        if (!it_channel->second.isUserInChannel(c)) {
            sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(),it_channel->second.get_channel_name()));
            return;
        }
        else {
            std::string modes = "+";
            if (it_channel->second.getIsInviteOnly())
                modes += "i";
            if (it_channel->second.getTopicRestriction())
                modes += "t";
            if (it_channel->second.getisLimited())
                modes += "l";
            if (it_channel->second.getIsKeySet())
                modes += "k";

            if (it_channel->second.getisLimited()) {
                std::stringstream n_obj;
                n_obj << it_channel->second.get_num_limite();
                modes += " " + n_obj.str();
            }
            if (it_channel->second.getIsKeySet()) {
                if (it_channel->second.isClientOperator(c))
                    modes += " " + it_channel->second.Get_key();
                else
                    modes += " *";
            }

            sendReply(c, error. RPL_CHANNELMODEIS(c.get_nickname(), 
            modes, it_channel->second.get_channel_name()));
            sendReply(c, error. RPL_CREATIONTIME (c.get_nickname(), 
            it_channel->second.fromTime(it_channel->second.getCreationTime()),
            it_channel->second.get_channel_name()));
        }
    }
    else {
        if (!isChannelExist(it_channel, cmds, c))
            return;
        channelCurrentModes.isInviteOnly = it_channel->second.getIsInviteOnly();
        channelCurrentModes.topicRestriction = it_channel->second.getTopicRestriction();
        channelCurrentModes.isLimited = it_channel->second.getisLimited();
        channelCurrentModes.num_limite = it_channel->second.get_num_limite();
        if (!it_channel->second.isUserInChannel(c)) {
            sendReply(c, error.ERR_NOTONCHANNEL(c.get_nickname(),
            it_channel->second.get_channel_name()));
            return;
        }
        if (!it_channel->second.isClientOperator(c)) {
            sendReply(c, error.ERR_CHANOPRIVSNEEDED(c.get_nickname(),
                      it_channel->second.get_channel_name()));
            return;
        }
        seccessModes.push_back("");
        modes = parseModes(cmds, c);
        if (modes.empty())
            return;
        bool ShouldSkeepKeys = false;
        bool ShouldSkeepLimts = false;
        for (size_t i = 0; i < modes.size(); i++) {
            if (modes[i].mode == 'k') {
                if (ShouldSkeepKeys)
                    continue ;
                else
                    ShouldSkeepKeys = true;
            }
            if (modes[i].mode == 'l') {
                if (!ShouldSkeepLimts) {
                    unsigned int limit = static_cast<unsigned int>(std::atol(modes[i].param.c_str()));
                    if (modes[i].sing && it_channel->second.getisLimited() && it_channel->second.get_num_limite() == limit)
                        continue ;
                    else if (!modes[i].sing && !it_channel->second.getisLimited())
                        continue ;
                    ShouldSkeepLimts = true;
                } else 
                    continue ;
            }
            if (it_channel->second.modeExecuter(modes[i], c, *this))
                filerM[modes[i].mode] = modes[i];
        }
        std::map<char, modes_t>::iterator hold;
        hold = filerM.find('i');
        if (hold != filerM.end()) {
            if (it_channel->second.getIsInviteOnly() != channelCurrentModes.isInviteOnly)
                prepareModesMessage(hold,sortModesPlus, sortModesMinus, seccessModes );
        }
        hold = filerM.find('t');
        if (hold != filerM.end()) {
            if (it_channel->second.getTopicRestriction() != channelCurrentModes.topicRestriction)
                prepareModesMessage(hold,sortModesPlus, sortModesMinus, seccessModes );
        }
        hold = filerM.find('l');
        if (hold != filerM.end() && !hold->second.sing) {
            if (it_channel->second.getisLimited() != channelCurrentModes.isLimited)
                prepareModesMessage(hold,sortModesPlus, sortModesMinus, seccessModes );
        }

        for (std::map<char,modes_t>::iterator it = filerM.begin(); it != filerM.end(); ++it) {
            if (it->second.mode == 'k' || (it->second.mode == 'l' && it->second.sing && !it->second.param.empty()) || it->second.mode == 'o')
                prepareModesMessage(it,sortModesPlus, sortModesMinus, seccessModes );
        }

        if (!sortModesPlus.empty())
            sortModesPlus = it_channel->second.removeDuplicate("+" + sortModesPlus);
        if (!sortModesMinus.empty())
            sortModesMinus = it_channel->second.removeDuplicate("-" + sortModesMinus);

        seccessModes[0] = sortModesMinus + sortModesPlus;

        for (size_t i = 0; i < seccessModes.size(); i++) {
            sortModes = sortModes + seccessModes[i];
            if (i < seccessModes.size() - 1)
                sortModes += " ";
        }

        if (!sortModes.empty()) {
            it_channel->second.broadcast(error.RPL_MODEOPTIONS(c.get_Prefix(), 
            it_channel->second.get_channel_name(), sortModes));
        }
    }
    modes.clear();
    filterdModes.clear();
}