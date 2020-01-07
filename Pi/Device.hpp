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

    public:
    Device(int, std::string);
    virtual ~Device();

        virtual void sendMsg(char*);
        virtual bool recvMsg(char*);
        virtual void operator()() = 0;

        int getSock();
        void setSock(int);

        virtual int getStatus() = 0;
};

#endif
