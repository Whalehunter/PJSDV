#include "Stoel.hpp"

Stoel::Stoel(int sock): Device{sock}
{
    this->name = "Stoel";
}

Stoel::~Stoel()
{
}

void Stoel::operator()()
{
    char buffer[256];

    strcpy(buffer, "getStatus\r");

    sendMsg(buffer);

    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::stringstream s(buffer);
        s >> knopValue;

        std::cout << "Stoel: " << knopValue << std::endl;

        strcpy(buffer, "getStatus\r");
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
