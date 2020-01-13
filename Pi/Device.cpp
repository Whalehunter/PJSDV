#include "Device.hpp"
#include <cstring>
//#include "nlohmann/json.hpp"

//using json = nlohmann::json;

Device::Device(int n, Appartement* ap): sock(n), a(ap), knopValue(0), sensorValue(0)
{
}

Device::~Device()
{
}

void Device::sendMsg(const char* data)
{
    const char* ss = strcat((char*)data,"\r");

    if (send(sock, ss, strlen(ss), 0) < 0) {
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
