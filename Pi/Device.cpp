#include "Device.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Stoel.hpp"
#include <cstring>
#include <iostream>

using namespace std;

Device::Device(int n, Appartement* a): sock(n), appartement(a)
{}

Device::~Device()
{}

void Device::sendMsg(char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        cout << "Error sending" << endl;
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
    this->sock = x;
}
