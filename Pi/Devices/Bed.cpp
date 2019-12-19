#include "Bed.hpp"

Bed::Bed(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Bed aangemaakt" << std::endl;
}

Bed::~Bed()
{
}

void Bed::operator()()
{
    char buffer[256];
    int timestamp = 0;
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(buffer)) {
        std::cout << "" << std::endl;
        timestamp++;

        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

void Bed::getStatus()
{
    std::cout << "anders" << std::endl;
}

