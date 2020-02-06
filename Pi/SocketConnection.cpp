#include "SocketConnection.hpp"

SocketConnection::SocketConnection(int sock, int buflen) : sockId(sock), buflen(buflen)
{
}

SocketConnection::~SocketConnection()
{}

void SocketConnection::sendBuffer(const char * buffer)
{
    if (send(sockId, buffer, strlen(buffer), 0) < 0) {
        std::cout << "Error sending on socket: " << sockId << std::endl;
    }
}

bool SocketConnection::receiveBuffer(char * buffer)
{
    return (recv(sockId, buffer, 255, 0) > 0);
}

int SocketConnection::getId()
{
    return sockId;
}
