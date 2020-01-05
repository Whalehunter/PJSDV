#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include <thread>

Appartement::Appartement()
{
    this->deviceNamePair = {};
}

void Appartement::registerDevice(std::string name, int sockId)
{

    if (id == 'x') {
        if(gui != 0) delete gui;

        gui = new Gui(sock, this);
        std::thread guiThread(&Device::operator(), gui);
        guiThread.detach();
    }
    else if (id == 'y') {
        if(bed != 0) delete bed;

        bed = new Bed(sock, this);
        std::thread bedThread(&Device::operator(), bed);
        bedThread.detach();
    }
    else if (id == 'z') {
        if(stoel != 0) delete stoel;

        stoel = new Stoel(sock, this);
        std::thread stoelThread(&Device::operator(), stoel);
        stoelThread.detach();
    }
}
