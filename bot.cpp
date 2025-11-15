#include "server/Client.hpp"
#include <algorithm>
#include <netinet/in.h>
#include <string>
#include <strings.h>
#include <sys/_endian.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

#define port 6667
#define  ip "127.0.0.1"
std::string get_sender_name(std::string &prfx)
{
    size_t ddot_posi = prfx.find(':');
    size_t mark_posi = prfx.find('!');
    if (ddot_posi == std::string::npos || mark_posi == std::string::npos)
    {
        return "";
    }
    // std::string sender_name = prfx.substr(ddot_posi+1,mark_posi);
    return (prfx.substr(0,mark_posi));
}
std::string get_message(std::string &prfx)
{
    size_t ddot_posi = prfx.find(':');
    if (ddot_posi == std::string::npos)
    {
        return "";
    }
    std::string arg = prfx.substr(ddot_posi+1);
    std::string cmd[5] = {"amine", "amine\n", "amine\r\n","amine\0","!help"};
    int i;
    for ( i = 0;cmd[i] != arg && i < 5;)
        i++;
    switch (i) {
        case 0:
         return "1";
        case 1:
         return "2";
        case 2:
         return "3";
        case 3:
         return "4";
        case 4:
         return "5";
        default:
         return "help message\n";
    }
}
int main(int ac,char **av)
{
    int boot_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_data;
    server_data.sin_family = AF_INET;
    server_data.sin_port = htons(port);
    server_data.sin_addr.s_addr = inet_addr(ip);
    if (connect(boot_fd, (const struct sockaddr *)&server_data, sizeof(sockaddr_in)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
    std::string pass_msg = "PASS amine\r\n";
    send(boot_fd, pass_msg.c_str(), pass_msg.length(), 0);

    std::string nick_msg = "NICK Bot\r\n";
    send(boot_fd, nick_msg.c_str(), nick_msg.length(), 0);

    std::string user_msg = "USER Bot 0 * :Bot\r\n";
    send(boot_fd, user_msg.c_str(), user_msg.length(), 0);

    while (true) 
    {
        char buffer[1024];

        bzero(buffer, 1024);
        int byte_recv = recv(boot_fd, buffer, 1000, 0);
        std::string recv_msg =  buffer;
        std::string sender;
    if (byte_recv >  0)
    {
        if (recv_msg.find("aut failed") != std::string::npos)
        {
            std::cerr << "error failed connection" <<std::endl;
            return 1;
        }
        else {
            sender = get_sender_name(recv_msg);
            // std::cout << "recv_msg is [" << recv_msg  << "]" <<std::endl;  
            // std::cout << "sender name id " << sender <<std::endl;  
            // std::cout << "cmd he want is  [" << get_message(recv_msg)<< "]" <<std::endl;  
            std::string to_send  = "privmsg " + sender + " :" + get_message(recv_msg) + "\r\n";
            // to_send = to_send.substr(0,to_send.length() );
            // to_send = "privmsg amine hhhh\r\n";

            std::cout << "to_send -> [" << to_send << "]"<<std::endl;
            send(boot_fd, to_send.c_str(), to_send.length(), 0);
            // get_message(recv_msg);

        }
    }
    else {
    
    }
    }
}