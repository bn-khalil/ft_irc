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

    char buffer[1024];

    bzero(buffer, 1024);
    int byte_recv = recv(boot_fd, buffer, 1000, 0);
    if (byte_recv >  0)
    {
        std::string recv_msg =  buffer;
        std::string sender;        
        //pass 462  461 464 // 431 433
        if (recv_msg.find(" 462") != std::string::npos || recv_msg.find(" 461") != std::string::npos || recv_msg.find(" 464 ") != std::string::npos|| recv_msg.find(" 431") != std::string::npos|| recv_msg.find(" 431") != std::string::npos|| recv_msg.find(" 433") != std::string::npos) 
        {
            
            std::cerr << "error failed connection"  << "recv_msg " << recv_msg <<std::endl;
            return 1;
        }
    }
    while (true) 
    {
        char buffer[1024];

        bzero(buffer, 1024);
        int byte_recv = recv(boot_fd, buffer, 1000, 0);
    if (byte_recv >  0)
    {
        std::string recv_msg =  buffer;
        std::string sender;
            sender = get_sender_name(recv_msg);

            std::string to_send  = "privmsg " + sender + " :" + get_message(recv_msg) + "\r\n";
            
            std::cout << "to_send -> [" << to_send << "]"<<std::endl;
            
            send(boot_fd, to_send.c_str(), to_send.length(), 0);
            // get_message(recv_msg);

    }
    else if(1) 
    {
    
    }
    }
}
