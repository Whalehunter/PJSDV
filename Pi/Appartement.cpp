#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Deur.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include "Devices/Zuil.hpp"
#include "Devices/Schemerlamp.hpp"

Appartement::Appartement(): deur(0), gui(0), bed(0), stoel(0), zuil(0)
{
    this->deviceNamePairs = {};
}

Appartement::~Appartement()
{
    delete deur;
    delete gui;
    delete stoel;
    delete bed;
    delete zuil;
}

Appartement* Appartement::getInstance()
{
    Device * ob = NULL;
    if (id == 'd') {
        if(deur != 0) delete deur;
        deur = new Deur(sock, this);
        ob = deur;
    }
    else if (id == 'f') {
        if(zuil != 0) delete zuil;
        zuil = new Zuil(sock, this);
        ob = zuil;
    }
    else if (id == 's') {
        if(schemerlamp != 0) delete schemerlamp;
        schemerlamp = new Schemerlamp(sock, this);
        ob = schemerlamp;
    }
    else if (id == 'x') {
       // if(gui != 0) delete gui;
       // gui = new Gui(sock, this);
        ob = new Gui(sock, this);
    }
    else if (id == 'y') {
        if(bed != 0) delete bed;
        bed = new Bed(sock, this);
        ob = bed;
    }
    else if (id == 'z') {
        if(stoel != 0) delete stoel;
        stoel = new Stoel(sock, this);
        ob = stoel;
    }

    if (ob != NULL) {
        devices.insert(std::pair<char, Device*>(id, ob));
        std::thread obThread(&Device::operator(), ob);
        obThread.detach();
    }

    this->deviceNamePairs.insert(std::pair<std::string, Device*>(capitalized, newDevice));
    std::thread newThread(&Device::operator(), newDevice);
    newThread.detach();

    return newDevice;
}



const Device * Appartement::deviceExists(std::string name) {
    auto device = this->deviceNamePairs.find(name);
    if (device == this->deviceNamePairs.end()) {
        return NULL;
    }
    return device->second;
}

void Appartement::registerDevice(std::string name, int sockId)
{
    // x = gui
    // d = deur
    if (id == 'x')
        if(gui != 0) delete gui;
        else if (id == 'y')
            if(bed != 0) delete bed;
            else if (id == 'z')
        if(stoel != 0) delete stoel;

}
