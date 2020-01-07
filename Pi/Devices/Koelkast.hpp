#ifndef KOELKAST_HPP
#define KOELKAST_HPP

#include "../Device.hpp"

class Koelkast: public Device
{
public:
    Koelkast(int sock);
    void operator()();
    int getStatus();
};

#endif
