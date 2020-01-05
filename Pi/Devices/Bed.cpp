#include "Bed.hpp"

Bed::Bed(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

void Bed::operator()()
{
    char buffer[256];

    strcpy(buffer, "getStatus\r");

    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::stringstream s(buffer);
        s >> knopValue;

        std::cout << "Bed: " << knopValue << std::endl;

        strcpy(buffer, "getStatus\r");
        sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

int Bed::getStatus()
{
    return knopValue;
}
