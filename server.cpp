#include <iostream>
#include <poll.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <unistd.h>

void f(){
    system("lsof -iTCP a.out");
}
class server
{
    private:
        int serverSocketFd;
        int port;
        
    public:
        server( int port ): port(port) {};
        ~server(){
            close(serverSocketFd);
        };

        void startServer() {
            struct sockaddr_in add;
	        struct pollfd Poll;

            add.sin_family = AF_INET;
            add.sin_port = htons(this->port);
            add.sin_addr.s_addr = INADDR_ANY;

            this->serverSocketFd = socket(AF_INET, SOCK_STREAM , 0);
            if (this->serverSocketFd <= -1)
                throw std::runtime_error("socket server not created!");
            if (bind(this->serverSocketFd, (struct sockaddr*)&add, sizeof(add)) < 0)
                throw std::runtime_error("bind failed");
            if (listen(this->serverSocketFd, SOMAXCONN) < 0)
                throw std::runtime_error("listen failed");
            Poll.events = POLLIN;
            Poll.fd = this->serverSocketFd;
            std::cout << "Server running on port " << this->port << "..." << std::endl;
            f();
            while (true)
                ;
            
        }
};


int main()
{
    atexit(f);
    server server(6667);
    server.startServer();
    return 0;
}
