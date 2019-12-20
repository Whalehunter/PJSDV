#include "Stoel.hpp"

Stoel::Stoel(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Stoel::~Stoel()
{
}

void Stoel::operator()()
{
    char buffer[256];

    strcpy(buffer, "getStatus");

    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::stringstream s(buffer);
        s >> knopValue;

        std::cout << knopValue << std::endl;

        strcpy(buffer, "getStatus");
        sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

int Stoel::getStatus()
{
    return knopValue;
}

