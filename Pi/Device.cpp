#include "Device.hpp"

Device::Device(int n): sock(n), knopValue(0), sensorValue(0)
{}

void Device::sendMsg(char* data)
{
    if (send(this->sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending" << std::endl;
    }
}

bool Device::recvMsg(char* data)
{
    return (recv(this->sock, data, 255, 0) > 0);
}

int Device::getSock()
{
    return this->sock;
}

void Device::setSock(int x)
{
    sock = x;
}

static void createInstance() {
}
