#include "SocketConnection.hpp"

SocketConnection::SocketConnection(int sock, int buflen) : sockId(sock), buflen(buflen)
{}

SocketConnection::~SocketConnection()
{}

/**
 * Stuur buffer over socket op basis van
 *
 * param buffer: Buffer om mee te sturen
 *
 * Return void
 *
 * Exception: locally detected errors zijn -1
 */
void SocketConnection::sendBuffer(const char * buffer)
{
    if (send(sockId, buffer, strlen(buffer), 0) < 0) {
        std::cout << "Error sending on socket: " << sockId << std::endl;
    }
}

/**
 * Ontvang buffer over socket op basis van socketId
 *
 * param buffer: Buffer om te vullen met ontvangen text
 *
 * Return bool: of er data is ontvangen
 */
bool SocketConnection::receiveBuffer(char * buffer)
{
    return (recv(sockId, buffer, 255, 0) > 0);
}

/**
 * Socket ID ophalen
 *
 * return int: Socket ID
 */
int SocketConnection::getId()
{
    return sockId;
}
