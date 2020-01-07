#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include "Devices/Schemerlamp.hpp"
#include "Devices/Koelkast.hpp"
#include "Devices/Zuil.hpp"
#include "Devices/Muur.hpp"
#include "Devices/Deur.hpp"
#include <thread>
#include <string>

Appartement::Appartement()
{
    this->deviceNamePairs = {};
}

Appartement::~Appartement()
{
    for (auto const& [name, pInstance] : this->deviceNamePairs) {
        delete pInstance;
    }
}

Appartement* Appartement::getInstance()
{
    if (NULL == instance) {
        instance = new Appartement();
    }
    return instance;
}

const Device* Appartement::getDevice(const std::string name, int sock = 0)
{
    std::string capitalized = this->capitalize(name);

    if ("Mary" == capitalized) {
    } else if ("Bewaker" == capitalized) {
    }

    if (sock == 0) {
        auto device = this->deviceExists(capitalized);
        if (device != NULL) {
            return device;
        }
    }

    Device * newDevice = NULL;

    if ("Bed" == capitalized) {
        newDevice = new Bed(sock);
    } else if ("Stoel" == capitalized) {
        newDevice = new Stoel(sock);
    } else if ("Muur" == capitalized) {
        newDevice = new Muur(sock);
    } else if ("Schemerlamp" == capitalized) {
        newDevice = new Schemerlamp(sock);
    } else if ("Zuil" == capitalized) {
        newDevice = new Zuil(sock);
    } else if ("Koelkast" == capitalized) {
        newDevice = new Koelkast(sock);
    } else if ("Deur" == capitalized) {
        newDevice = new Deur(sock);
    }

    this->deviceNamePairs.insert(std::pair<std::string, Device*>(capitalized, newDevice));

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
