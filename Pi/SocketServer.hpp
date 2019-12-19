#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>

class SocketServer
{
    public:
        SocketServer(Appartement*);
        ~SocketServer();
        
        void start();
        void accept();
    private:
        Appartement* appartement;

        int csock;
        int sockfd;
        struct sockaddr_in server_addr;
        void handshake(int);
};

#endif
