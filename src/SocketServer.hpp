#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include "Appartement.hpp"

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
        SocketServer(const Appartement&);
        ~SocketServer();
        
        void startServer();
        void acceptConnection();
    private:
        Appartement a;

        int csock;
        int sockfd;
        struct sockaddr_in server_addr;
        void handshake(int);
        unsigned int socksize;
};

#endif
