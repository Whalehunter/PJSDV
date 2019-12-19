#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "SocketServer.hpp"
class Appartement;

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

        virtual void getStatus() = 0;
};

#endif
