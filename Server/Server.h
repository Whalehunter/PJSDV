#ifndef SERVER_H
#define SERVER_H

#include <cstring> // strlen, strcat enzo
#include <netdb.h> // sockets
#include <sys/socket.h> // sockets
#include <unistd.h> // close
#include <iostream> // cout
#include <thread> // threads

using namespace std;

class Server
{
    public:
        Server();

        void acceptConnection();
        void sendMessage(char*, int);
        bool recvMessage(char*, int);

    private:
        int csock;
        int sockfd;
        int n;
        char buffer[256];
        struct sockaddr_in server_addr;
        unsigned int socksize;

        void threadHandler(int);
        void setupServer();
};

#endif // SERVER_H
