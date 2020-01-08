#include "Bed.hpp"

Bed::Bed(int socketId): Device(socketId)
{
    this->name = "Bed";
    std::cout << "Bed aangemaakt" << std::endl;
}

void Bed::operator()()
{
    Appartement::getInstance()->getDevice()
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
