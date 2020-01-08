#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Appartement.hpp"
#include "nlohmann/json.hpp"
//#include <cstring>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <ctime>

//using json = nlohmann::json;

class Device
{
    protected:
        int sock;
        int knopValue;
        int sensorValue;

        Appartement* a;
    public:
        Device(int, Appartement*);
        virtual ~Device();

        virtual void sendMsg(char*);
        virtual bool recvMsg(char*);
        virtual void operator()() = 0;
        
//        json dicks = {{"one","two"}};

        int getSock();
        void setSock(int);

        virtual nlohmann::json getStatus() = 0;
};

#endif
