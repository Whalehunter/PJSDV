#ifndef STOEL_HPP
#define STOEL_HPP

#include "../Device.hpp"

class Stoel: public Device
{
    public:
    Stoel(int sock);
        ~Stoel();

        void operator()();
        int getStatus();
};

#endif
