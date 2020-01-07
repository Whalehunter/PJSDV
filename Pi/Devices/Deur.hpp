#ifndef DEUR_HPP
#define DEUR_HPP

#include "../Device.hpp"

class Deur: public Device
{
public:
    Deur(int sock);
    void operator()();
    int getStatus();
};

#endif
