#include "Stoel.hpp"

Stoel::Stoel(int n, Appartement* ap): Device(n, ap)
{
    std::cout << "Stoel aangemaakt" << std::endl;
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

        std::cout << "Stoel: " << knopValue << std::endl;

        strcpy(buffer, "getStatus");
        sendMsg(buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    std::cout << "Connection closed on socket " << sock << std::endl;
}

nlohmann::json Stoel::getStatus()
{
   nlohmann::json data = {"Placeholder", 0};
   return data;
   // return knopValue;
}
