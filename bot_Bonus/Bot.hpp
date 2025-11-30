#ifndef BOT_HPP
#define BOT_HPP

#include <sstream>
#include <string>
#include <sys/fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/syslimits.h>
#include <cctype>
#include <unistd.h>

int stringToPort(std::string string);
bool isValidPassword(std::string &str);
bool isValidNickname(const std::string &nick);

class Bot
{
private:
    std::string serverIp;
    int         port;
    std::string password;
    std::string nick;
    int         BotFd;
    std::string buffer;
    std::string authCmds;

public:
    Bot(std::string password, int port, std::string serverIp, std::string nick);
    void connectServer();
    void run();
    void hundleLineMsg(std::string line);
};

#endif