#include "Appartement.hpp"
#include "Device.hpp"
#include "Devices/Deur.hpp"
#include "Devices/Gui.hpp"
#include "Devices/Bed.hpp"
#include "Devices/Stoel.hpp"
#include "Devices/Zuil.hpp"
#include "Devices/Schemerlamp.hpp"
#include "Devices/Muur.hpp"
#include "Devices/Koelkast.hpp"

Appartement::Appartement()
{}

Appartement::~Appartement()
{
    std::map<char, Device*>::iterator deviceIterator = devices.begin();
    while (deviceIterator != devices.end()) {
        delete deviceIterator->second;
    }
}

bool Appartement::createDevice(int sock, char id)
{
    Device * ob = NULL;
    switch (id) {
    case 'd': ob = new Deur(sock, this); break;
    case 'f': ob = new Zuil(sock, this); break;
    case 'm': ob = new Muur(sock, this); break;
    case 'k': ob = new Koelkast(sock, this); break;
    case 's': ob = new Schemerlamp(sock, this); break;
    case 'y': ob = new Bed(sock, this); break;
    case 'z': ob = new Stoel(sock, this); break;
    case 'x':
        Gui * gui = new Gui(sock, devices);
        guis.push_back(gui);
        std::thread guiThread(&Gui::operator(), gui);
        guiThread.detach();
        return true;
    }

    if (ob != NULL) {
        devices.insert(std::pair<char, Device*>(id, ob));
        std::thread obThread(&Device::operator(), ob);
        obThread.detach();
        return true;
    }

    return false;
}
