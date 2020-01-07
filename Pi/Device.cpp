#include "Device.hpp"

Device::Device(int socketId): sock(socketId), knopValue(0), sensorValue(0)
{}

void Device::sendMsg(char* data)
{
    if (send(this->sock, data, strlen(data), 0) < 0) {
        std::cout << "Error sending message van " << this->name << " on socket id:" << this->sock << std::endl;
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

void Device::handshake()
{
    char send[] = "ID?\r";
    char buf[255];
    this->sendMsg(send);
    this->recvMsg(buf);

}
