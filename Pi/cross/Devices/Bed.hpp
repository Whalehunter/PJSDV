#ifndef BED_HPP
#define BED_HPP

#include "../Device.hpp"

class Bed: public Device
{
    public:
        Bed(int, Appartement*);
        ~Bed();

        void operator()();
        int getStatus();
};

#endif
