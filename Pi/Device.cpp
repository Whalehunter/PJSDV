#include "Device.hpp"

Device::Device(int n, Appartement* ap): sock(n), a(ap), knopValue(0), sensorValue(0), timer(0)
{}

Device::~Device()
{}

void Device::sendMsg(const char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending on socket: " << sock << std::endl;
    }
}

bool Device::recvMsg(char* data)
{
    return (recv(sock, data, 255, 0) > 0);
}

int Device::getSock()
{
    return sock;
}

void Device::setSock(int x)
{
    sock = x;
}

bool Device::compareTime(std::clock_t timer, double timeLimit)
{
    /* compare timer with set time, return true/false depending on whether
     * it has passed the configured time limit
     */
    return (((std::clock() - timer) / (double) CLOCKS_PER_SEC) >= timeLimit);
}

int Device::getSensor()
{
    return sensorValue;
}

int Device::getKnop()
{
    return knopValue;
}
