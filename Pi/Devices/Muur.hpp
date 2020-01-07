#ifndef MUUR_HPP
#define MUUR_HPP

#include "../Device.hpp"

class Muur: public Device
{
public:
    Muur(int sock);
    void operator()();
    int getStatus();
};

#endif
