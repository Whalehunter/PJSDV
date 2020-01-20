#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Appartement.hpp"
#include "nlohmann/json.hpp"
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ctime>

class Device
{
protected:
    int sock;
    Appartement* a;
    int knopValue;
    int sensorValue;


public:
    Device(int, Appartement*);
    virtual ~Device();

    virtual void sendMsg(const char*);
    virtual bool recvMsg(char*);
    virtual void operator()() = 0;

    int getSock();
    void setSock(int);
    int getSensor();
    int getKnop();
    virtual nlohmann::json getStatus() = 0;
};

#endif
