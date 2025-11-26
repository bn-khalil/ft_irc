#include "Bot.hpp"

// Helper functions implementation
int stringToPort(std::string string)
{
    int port = -1;
    std::stringstream str_strm;

    str_strm << string;
    str_strm >> port;

    if (str_strm.fail() || !str_strm.eof())
        return -1;
    else if (port >= 1024 && port <= 49151)
        return port;
    return -1;
}

bool isValidPassword(std::string &str)
{
    std::string W_spaces = " \t\n\r\v\f";
    if (str.empty() || str.length() > 100)
        return false;

    for(int i = 0; str[i] ; i++)
    {
        if (W_spaces.find(str[i]) != std::string::npos)
            return false;
    }
    return true;
}

bool isValidNickname(const std::string &nick)
{
    if (nick.empty())
        return false;

    std::string forbiddenStartChars = "0123456789$:#&";

    if (forbiddenStartChars.find(nick[0]) != std::string::npos)
        return false;

    std::string forbiddenChars = " ,*?!@";

    if (nick.find_first_of(forbiddenChars) != std::string::npos)
        return false;

    return true;
}

// Bot Class implementation
Bot::Bot(std::string password, int port, std::string serverIp, std::string nick)
{
    if (this->port < 0)
        throw std::runtime_error("Error: Invalid port number provided");
    else if (isValidPassword(password) == false)
        throw std::runtime_error("Error: Invalid password format ");
    else if (isValidNickname(nick) == false)
        throw std::runtime_error("Error: Invalid nick name ");

    this->password = password;
    this->port = port;
    this->serverIp = serverIp;
    this->nick = nick;
    BotFd = -1;
    authCmds = "PASS " + password + "\r\n" + "NICK " + nick + "\r\n" + "USER " + nick + " 0 * :Bot User\r\n";
}

void Bot::connectServer()
{
    struct sockaddr_in serv_addr;
    
    BotFd = socket(AF_INET, SOCK_STREAM, 0);
    if (BotFd < 0)
        throw std::runtime_error("Socket creation failed");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(serverIp.c_str());

    if (connect(BotFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection failed");

    std::cout << " Connected to server! " << std::endl;
    
    send(BotFd,authCmds.c_str(),authCmds.length(),0);
    
    char check_auth[1024];
    bzero(check_auth, 1024);

    int bytes = recv(BotFd, check_auth, 1023, 0);

    if (bytes > 0)
    {
        std::string response = check_auth;

        if (response.find(" 464 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Incorrect Password.");

        if (response.find(" 433 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Nickname '" + nick + "' is already in use.");

        if (response.find(" 461 ") != std::string::npos) 
            throw std::runtime_error("Error: Authentication Failed - Not enough parameters.");
    }
}

void Bot::run()
{
    char buffer_char[1024];

    while (true) 
    {
        bzero(buffer_char, sizeof(buffer_char) );    
        int bytesReceived = recv(BotFd, buffer_char, sizeof(buffer_char) -1, 0);

        if (bytesReceived <= 0)
        {
            throw std::runtime_error("Error: Server disconnected.");
        }

        buffer.append(buffer_char);

        size_t pos;
        while ((pos = buffer.find("\r\n")) != std::string::npos)
        {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 2);
            hundleLineMsg(line); 
        }
    }
}

void Bot::hundleLineMsg(std::string line)
{
    if (line.find("PRIVMSG") != std::string::npos) 
    {
        size_t exclamation = line.find('!');
        size_t colon = line.find(':', 1);

        if (exclamation != std::string::npos && colon != std::string::npos) 
        {
            std::string sender = line.substr(1, exclamation - 1);
            std::string msg = line.substr(colon + 1);
            std::string to_send;

            if (msg == "!help") 
                to_send = "Available commands: !help, !hello";
            else if (msg == "!hello")
                to_send = "Available commands: !help, !hello";
            std::string cmd = "PRIVMSG " + sender + " :" + to_send + "\r\n";
            send(BotFd,cmd.c_str(),cmd.length(),0);
        }
    }
}

int main(int ac, char **av)
{
    if (ac != 5)
    {
        std::cerr << "./bot <password of server> <Port> <IP of server> <nickname of Bot>" << std::endl;
        return 1;
    }
    try {
        Bot bot(av[1], atoi(av[2]), av[3],av[4]);
        bot.connectServer();
        bot.run();

    } catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}