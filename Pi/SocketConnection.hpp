#ifndef SOCKET_CONNECTION_HPP
#define SOCKET_CONNECTION_HPP

#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cstring>
#include <sstream>
#include <sys/socket.h>

class SocketConnection
{
    int sockId;                 // Socket ID
    int buflen;                 // Buffer length om mee te sturen over socket
public:
    SocketConnection(int, int);
    ~SocketConnection();
    void sendBuffer(const char*);
    bool receiveBuffer(char*);
    int getId();
};

#endif
