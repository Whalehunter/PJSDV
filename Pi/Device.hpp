#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Appartement.hpp"
#include "SocketConnection.hpp"
#include "nlohmann/json.hpp"
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ctime>

class Device
{
protected:
    SocketConnection socket;
    Appartement* a;
    int knopValue;
    int sensorValue;
    std::clock_t timer;

public:
    Device(int, Appartement*);
    virtual ~Device();

    virtual void operator()() = 0;

    int getSensor();
    int getKnop();
    bool compareTime(std::clock_t, double);

    virtual nlohmann::json getStatus() = 0;
};

#endif
