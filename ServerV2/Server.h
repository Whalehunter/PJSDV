#ifndef SERVER_H
#define SERVER_H

#include <cstring> // strlen, strcat enzo
#include <netdb.h> // sockets
#include <sys/socket.h> // socket
#include <arpa/inet.h>
#include <unistd.h> // close
#include <iostream> // cout
#include <thread> // threads

class Server
{
    public:
        Server();

        void acceptConnection();
        void sendMsg(int, char*);
        bool recvMsg(int, char*);

    private:
        int csock;
        int sockfd;
        struct sockaddr_in server_addr;
        unsigned int socksize;

        void setupServer();
};

#endif // SERVER_H
