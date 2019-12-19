#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

#include "Device.hpp"
#include <string>
class Device;

class Appartement
{
    public:
        Appartement();
        ~Appartement();

        Device* device;

        void createDevice(int, std::string);
};

#endif

