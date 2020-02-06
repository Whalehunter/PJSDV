#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include "Appartement.hpp"

#include <cstring>      // strcpy, etc
#include <sys/socket.h> // sockets
#include <arpa/inet.h>  // address structs
#include <unistd.h>     // close(file descriptor)
#include <iostream>     // cout, endl
#include <thread>       // threads

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
