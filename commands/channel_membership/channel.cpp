/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akella <akella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 12:05:30 by akella            #+#    #+#             */
/*   Updated: 2025/11/22 12:05:31 by akella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel.hpp"
#include <sstream>
#include <sys/socket.h>
#include <sys/poll.h>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>

Channel::Channel(std::string name) : Channel_name(name)
{
    this->isKeySet = false;
    this->isInviteOnly = false;
    this->limit = false;
    this->num_limite = 0;
    this->key = "";
    this->isLimited = false;
    this->topicRestriction = true;
    this->channelTopic = "";
    this->time_creat_channel = time(0);
    this->channelActiveModes = "+t";
}
Channel::Channel() : Channel_name("Default_Channel_name")
{
    this->isKeySet = false;
    this->isInviteOnly = false;
    this->limit = false;
    this->num_limite = 0;
    this->key = "";
    this->isLimited = false;
    this->topicRestriction = true;
    this->channelTopic = "";
    this->time_creat_channel = time(0);
}
void Channel::removeClientFromOneChannels(Client &c)
{
    users.erase(c.get_nickname());
    operators_.erase(c.get_nickname());
    invite.erase(c.get_nickname());
}
    
std::string Channel::getNamesList()
{
    std::string name = "";
    for (std::map<std::string, Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (operators_.find(it->first) != operators_.end())
            name = name + '@';
        name += it->first;
        name += " ";
    }
    if (!name.empty())
        name.erase(name.length() - 1, 1);
    return name;
} 

bool Channel::getTopicRestriction() {
    return this->topicRestriction;
}

bool Channel::getisLimited() {
    return this->isLimited;
}

bool Channel::getIsKeySet() {
    return this->isKeySet;
}

bool Channel::getIsInviteOnly() {
    return this->isInviteOnly;
}

void Channel::Add_to_admin(Client &c)
{
    operators_.insert(std::make_pair(c.get_nickname(), &c));
}

void Channel::Add_to_user(Client &c)
{
    users.insert(std::make_pair(c.get_nickname(), &c));
}

void Channel::Add_to_invite(Client &c)
{
    invite.insert(std::make_pair(c.get_nickname(), &c));
}

std::string Channel::Get_key()
{
    return this->key;
}

bool Channel::isClientOperator(Client &c) const
{
    return (operators_.find(c.get_nickname()) != operators_.end()); 
}

bool Channel::isClientUSER(Client &c) const
{
    return (operators_.find(c.get_nickname()) != operators_.end()); 
}

std::string Channel::Set_key()
{
    return this->key;
}

bool Channel::isInvited(Client &c)
{
    return (invite.find(c.get_nickname()) != invite.end());
}

bool Channel::is_full()
{
    return (users.size() >= get_num_limite());
}

bool Channel::Check_mode(char mode)
{
    if (mode == 'k')
        return this->isKeySet;
    else if (mode == 'i')
        return this->isInviteOnly;
    else if (mode == 'l')
        return this->limit;
    return false;
}

bool Channel::isEmpty()
{
    if (users.size() == 0)
        return true;
    return false;
}

std::vector<std::string> Channel::split_comma(std::string &strr, char d)
{
    std::string save;
    std::stringstream ss(strr);
    std::vector<std::string> resulte;

    while (getline(ss, save, d))
    {
        resulte.push_back(save);
    }
    return resulte;
}

std::string Channel::to_lower(std::string str)
{
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

void Channel::setTopic(std::string topic) {
    this->channelTopic = topic;
}

std::string Channel::getTopic() {
    return this->channelTopic;
}

bool Channel::isUserInChannel(Client &c)
{
    std::map<std::string, Client*>::iterator it = users.find(c.get_nickname());
    if (it != users.end())
    {
        return true;
    }
    return false;
}

void Channel::broadcast(const std::string &msg)
{
    std::string full_msg = msg + "\r\n";
    for (std::map<std::string, Client*>::iterator it = users.begin(); it != users.end(); it++)
    {
        Client *c = it->second;
        send(c->getfd(), full_msg.c_str(), full_msg.length(), 0);
    }
}

void Channel::broadcastExpectSender(const std::string &msg, Client &sender)
{
    std::string full_msg = msg + "\r\n";
    for (std::map<std::string, Client*>::iterator it = users.begin(); it != users.end(); it++)
    {
        Client *c = it->second;
        if (c->get_nickname() == sender.get_nickname())
            continue;
        send(c->getfd(), full_msg.c_str(), full_msg.length(), 0);
    }
}

size_t Channel::get_number_of_users()
{
    return (users.size());
}

void Channel::setChannelActiveModes(std::string modes) {
    this->channelActiveModes = modes;
}
std::string  Channel::getChannelActiveModes() {
    return this->channelActiveModes;
}

time_t Channel::getCreationTime() {
    return this->time_creat_channel;
}

void Channel::setTimeTopic(time_t topicTime) {
    this->time_last_topic = topicTime;
}
time_t Channel::getTimeTopic() {
    return this->time_last_topic;
}
std::string Channel::fromTime(time_t time) {
    std::stringstream s_object;
    s_object <<  time;
    return s_object.str();
}