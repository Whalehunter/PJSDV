#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Appartement.hpp"
//#include "Devices/Gui.hpp"
#include <cstring>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

class Device
{
    protected:
        int sock;
        Appartement* a;
    public:
        Device(int, Appartement*);
        virtual ~Device();

        virtual void sendMsg(char*);
        virtual bool recvMsg(char*);
        virtual void operator()() = 0;

        int getSock();
        void setSock(int);

        virtual void getStatus() = 0;
};

#endif
