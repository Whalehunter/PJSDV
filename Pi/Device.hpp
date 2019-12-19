#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Appartement.hpp"

class Device
{
    protected:
        int sock;
        Appartement* appartement;
    public:
        Device(int, Appartement*);
        virtual ~Device();

        virtual void sendMsg(char*);
        virtual bool recvMsg(char*);
        virtual void operator()() = 0;

        int getSock();
        void setSock(int);

        void getStatus();
};

#endif
