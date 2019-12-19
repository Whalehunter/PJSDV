#ifndef APPARTEMENT_HPP
#define APPARTEMENT_HPP

//#include "Device.hpp"
//#include "Devices/Gui.hpp"
#include <string>
#include <iostream>
#include <thread>
class Device;
class Appartement
{
    public:
        Appartement();
        ~Appartement();

        Device* gui;
        Device* bed;
        Device* stoel;

        void createDevice(int, char);
};

#endif

