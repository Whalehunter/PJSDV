#include "Device.hpp"
//#include "nlohmann/json.hpp"

//using json = nlohmann::json;

Device::Device(int n, Appartement* ap): sock(n), a(ap), knopValue(0), sensorValue(0)
{
}

Device::~Device()
{
}

void Device::sendMsg(char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending on socket: " << sock << std::endl;
    }
}

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
    return this->sock;
}

void Device::setSock(int x)
{
    sock = x;
}
