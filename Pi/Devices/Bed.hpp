#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"

class Bed: public Device
{
    public:
    Bed(int sock);
    void operator()();
    int getStatus();
};

#endif
