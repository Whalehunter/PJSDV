#include "Device.hpp"
//#include "nlohmann/json.hpp"

//using json = nlohmann::json;

Device::Device(int socketId): sock(socketId), knopValue(0), sensorValue(0)
{}

void Device::sendMsg(const char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending on socket: " << sock << std::endl;
    }
}

void Device::recvMsg(char* data)
{
    if (recv(this->sock, data, 255, 0) < 1) {
        std::cout << this->name << " disconnected from socket " << this->sock << std::endl;
        close(sock);
    }
}

int Device::getSock()
{
    return this->sock;
}

void Device::setSock(int x)
{
    this->sock = x;
}

int Device::getSensor()
{
    return sensorValue;
}

int Device::getKnop()
{
    return knopValue;
}
