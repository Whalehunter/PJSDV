#ifndef STOEL_HPP
#define STOEL_HPP

#include "../Device.hpp"

class Stoel: public Device
{
    public:
        Stoel(int, Appartement*);
        ~Stoel();

        void operator()();
        nlohmann::json getStatus();
};

#endif
