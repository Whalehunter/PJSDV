#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include <string>

class Appartement
{
    public:
        Appartement();
        ~Appartement();

        Device* device;

        void createDevice(int, string);
}

#endif

