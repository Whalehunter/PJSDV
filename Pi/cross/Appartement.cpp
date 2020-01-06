#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Deur.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include "Devices/Zuil.hpp"

Appartement::Appartement(): deur(0), gui(0), bed(0), stoel(0), zuil(0)
{
}

Appartement::~Appartement()
{
    delete deur;
    delete gui;
    delete stoel;
    delete bed;
    delete zuil;
}

void Appartement::createDevice(int sock, char id)
{
    if (id == 'd') {
        if(deur != 0) delete deur;

        deur = new Deur(sock, this);
        std::thread deurThread(&Device::operator(), deur);
        deurThread.detach();
    }
    else if (id == 'f') {
        if(zuil != 0) delete zuil;

        zuil = new Zuil(sock, this);
        std::thread zuilThread(&Device::operator(), zuil);
        zuilThread.detach();
    }
    else if (id == 'x') {
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
