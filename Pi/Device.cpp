#include "Device.hpp"

Device::Device(int n, Appartement* ap): sock(n), a(ap)
{
}

Device::~Device()
{
}

void Device::sendMsg(char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending" << std::endl;
    }
}

bool Device::recvMsg(char* data)
{
    return (recv(sock, data, 255, 0) > 0);
}

int Device::getSock()
{
    return this->sock;
}

void Device::setSock(int x)
{
    sock = x;
}

