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

class Gui: public Device
{
    public:
        Gui(int, Appartement*);
        ~Gui();

        void operator()();
};

class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
};

class Stoel: public Device
{
    public:
        Stoel(int, Appartement*);
        ~Stoel();

        void operator()();
};

#endif
