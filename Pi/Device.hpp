#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstring>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <unistd.h>

class Device
{
protected:
    std::string name;
    int sock;
    int knopValue;
    int sensorValue;

    void handshake();

public:
    Device(int);
    virtual ~Device();

    void sendMsg(char*);
    void recvMsg(char*);
    virtual void operator()() = 0;

    int getSock();
    void setSock(int);

    virtual int getStatus() = 0;
};

#endif
